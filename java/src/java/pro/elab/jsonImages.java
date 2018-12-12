/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import java.io.File;
import java.lang.reflect.Type;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.stream.Stream;
import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;

/**
 * REST Web Service
 *
 * @author eugine
 */
@Path("images")
public class jsonImages {

    @Context
    private UriInfo context;
    @Context
    private ServletContext sContext;
    private Properties properties = new Properties();

    /**
     * Creates a new instance of jsonImages
     */
    public jsonImages() {
    }

    private void loadProperties() {
        try {
            properties.load(sContext.getResourceAsStream("/WEB-INF/imageupload.properties"));
            properties.setProperty("path", sContext.getRealPath("/"));
        } catch (Exception ex) {
            Logger.getLogger(jsonImages.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * Method POST Upload images in PNG. POST body: - JSON
     * {{content=#base64_image_content#|url=#url#}}
     *
     * \@usage http://localhost/api/images
     *
     * @param content representation for the resource
     * @return JSON array of filenames
     */
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    public String postJson(String content) {
        Logger.getLogger(jsonImages.class.getName()).log(Level.INFO, "POST JSON");
        if (properties.isEmpty()) {
            loadProperties();
        }
        List<String> images = new ArrayList<String>();
        Gson gson = new Gson();
        Type itemsArrType = new TypeToken<JSONFile[]>() {
        }.getType();
        JSONFile[] Items = gson.fromJson(content, itemsArrType);
        for (int i = 0; i < Items.length; i++) {
            if (Items[i].content != null && Items[i].content.length() > 0) {
                int fileId = new ImageFile(properties).saveContent(Items[i].content);
                images.add(String.valueOf(fileId));
            }
            if (Items[i].url != null && Items[i].url.length() > 0) {
                int fileId = new ImageFile(properties).download(Items[i].url);
                images.add(String.valueOf(fileId));
            }
        }
        return gson.toJson(images).toString();
    }

    /**
     * Method POST Upload images in PNG. POST body: - multipart/encoded files -
     * url=#url#
     *
     * \@usage http://localhost/api/images
     *
     * @param request HTTP Request
     * @return JSON array of filenames
     */
    @POST
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    @Produces(MediaType.APPLICATION_JSON)
    public String postMP(@Context HttpServletRequest request) {
        List<String> images = new ArrayList<String>();
        Gson gson = new Gson();
        if (properties.isEmpty()) {
            loadProperties();
        }
        if (ServletFileUpload.isMultipartContent(request)) {
            Logger.getLogger(jsonImages.class.getName()).log(Level.INFO, "POST MP");
            FileItemFactory factory = new DiskFileItemFactory();
            ServletFileUpload fileUpload = new ServletFileUpload(factory);
            try {
                List<FileItem> items = fileUpload.parseRequest(request);
                if (items != null) {
                    Iterator<FileItem> iter = items.iterator();
                    /*
                     * Return true if the instance represents a simple form
                     * field. Return false if it represents an uploaded file.
                     */
                    int fileId = 0;
                    while (iter.hasNext()) {
                        final FileItem item = iter.next();
                        final String itemName = item.getName();
                        final String fieldName = item.getFieldName();
                        if (fieldName.equals("url")) {
                            final String fieldValue = item.getString();
                            fileId = new ImageFile(properties).download(fieldValue);
                        } else {
                            fileId = new ImageFile(properties).saveFile(item.getInputStream());
                        }
                        images.add(String.valueOf(fileId));
                    }
                }
            } catch (Exception ex) {
                Logger.getLogger(jsonImages.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        return gson.toJson(images).toString();
    }

    /**
     * Method GET Getting array of images (JSON)
     *
     * \@usage http://localhost:8080/api/images
     *
     * @return JSON array of filenames
     */
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public String getJson() {
        if (properties.isEmpty()) {
            loadProperties();
        }
        Gson gson = new Gson();
        List<String> images = new ArrayList<String>();
        String fullPath = sContext.getRealPath("/") + properties.getProperty("imagesDir");
        // List of images
        try {
            try (Stream<java.nio.file.Path> filePathStream = Files.walk(Paths.get(fullPath))) {
                filePathStream.forEach(filePath -> {
                    if (Files.isRegularFile(filePath)) {
                        images.add(filePath.toString().substring(fullPath.length()));
                    }
                });
            }
        } catch (Exception ex) {
            images.add(ex.toString());
            Logger.getLogger(jsonImages.class.getName()).log(Level.SEVERE, null, ex);
        }
        return gson.toJson(images).toString();
    }

    /**
     * Method GET Getting an image
     *
     * \@usage http://localhost:8080/api/images/{id}
     *
     * @param id file's id
     * @return binary image
     */
    @GET
    @Path("/{id}")
    public Response getJson(@PathParam("id") int id) {
        if (properties.isEmpty()) {
            loadProperties();
        }
        Gson gson = new Gson();
        List<String> images = new ArrayList<String>();
        String fullPath = sContext.getRealPath("/") + properties.getProperty("imagesDir");

        // We need specific image
        File image = new File(fullPath + "/" + id);
        String result;
        if (!image.exists()) {
            ResponseBuilder response = Response.status(404);
            return response.build();
        }
        ResponseBuilder response = Response.ok(image);
        response.type("image/png");
        response.header("Content-Disposition", "attachment; filename=\"" + id + ".png\"");
        return response.build();

    }

}
