/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import com.google.gson.Gson;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.ExecutorService;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.stream.Stream;
import javax.annotation.PostConstruct;
import javax.servlet.ServletContext;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PUT;
import javax.ws.rs.PathParam;
import javax.ws.rs.QueryParam;
import javax.ws.rs.container.AsyncResponse;
import javax.ws.rs.container.Suspended;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.MultivaluedMap;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import org.jboss.resteasy.plugins.providers.multipart.InputPart;
import org.jboss.resteasy.plugins.providers.multipart.MultipartFormDataInput;
import org.apache.commons.io.IOUtils;

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
        } catch (Exception ex) {
            Logger.getLogger(jsonImages.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

//    /**
//     * POST method for updating or creating an instance of jsonImages
//     *
//     * @param content representation for the resource
//     */
//    @POST
//    @Consumes(MediaType.APPLICATION_JSON)
//    public String postJson(String content) {
//        return "[" + content.length() + "]";
//    }
    /**
     * POST method for updating or creating an instance of jsonImages
     *
     * @param MultipartFormDataInput
     * @return
     */
    @POST
    @Consumes(MediaType.MULTIPART_FORM_DATA)
    public Response postJson(MultipartFormDataInput input) {
        File local;
        //Get API input data
        Map<String, List<InputPart>> uploadForm = input.getFormDataMap();

        if (properties.isEmpty()) {
            loadProperties();
        }

        //The file name
        String fileName;
        String pathFileName = "";

        //Get file data to save
        List<InputPart> inputParts = uploadForm.get("attachment");

        try {
            for (InputPart inputPart : inputParts) {
                //Use this header for extra processing if required
                MultivaluedMap<String, String> header = inputPart.getHeaders();
                // convert the uploaded file to input stream
                InputStream inputStream = inputPart.getBody(InputStream.class, null);
                byte[] bytes = IOUtils.toByteArray(inputStream);
                // constructs upload file path
                new ImageFile(properties).saveFile(bytes);
            }
        } catch (Exception e) {
            e.printStackTrace();
            return Response.serverError().build();
        }
        return Response.status(201).entity(pathFileName).build();
    }

    /**
     * Method GET Getting array of images (JSON)
     *
     * @usage http://localhost:8080/api/images
     *
     * @return an instance of java.lang.String
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
     * @usage http://localhost:8080/api/images/{id}
     *
     * @param id
     * @return an instance of java.lang.String
     */
    @GET
    @Path("/{id}")
    @Produces(MediaType.APPLICATION_JSON)
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
