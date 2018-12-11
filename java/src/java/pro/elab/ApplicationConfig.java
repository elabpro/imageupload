/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Properties;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.stream.Stream;
import javax.annotation.PostConstruct;
import javax.servlet.ServletContext;
import javax.ws.rs.core.Application;
import javax.ws.rs.core.Context;

/**
 *
 * @author eugine
 */
@javax.ws.rs.ApplicationPath("api")
public class ApplicationConfig extends Application {

    @Context
    private ServletContext sContext;

    @Override
    public Set<Class<?>> getClasses() {
        Set<Class<?>> resources = new java.util.HashSet<>();
        addRestResourceClasses(resources);
        return resources;
    }

    /**
     * Check
     */
    @PostConstruct
    public void init() {
        Properties properties = new Properties();
        System.out.println("Initializing servlet");
        try {
            properties.load(sContext.getResourceAsStream("/WEB-INF/imageupload.properties"));
        } catch (Exception ex) {
            Logger.getLogger(jsonImages.class.getName()).log(Level.SEVERE, null, ex);
        }
        int maxId = 0;
        System.out.println("Checking images in " + properties.getProperty("imagesDir"));
        String fullPath = sContext.getRealPath("/") + properties.getProperty("imagesDir");
        // List of images
        try {
            Stream<java.nio.file.Path> filePathStream = Files.walk(Paths.get(fullPath));
            filePathStream.forEach(filePath -> {
                if (filePath.toFile().exists() && Files.isRegularFile(filePath)) {
                    if (filePath.toString().contains(".lock")) {
                        String fileImage = filePath.toString().substring(0, filePath.toString().indexOf(".lock"));
                        File fI = new File(fileImage);
                        try {
                            System.out.println("Removing " + fileImage);
                            fI.delete();
                        } catch (Exception ex) {
                            //
                        }
                        try {
                            System.out.println("Removing " + filePath);
                            filePath.toFile().delete();
                        } catch (Exception ex) {
                            //
                        }
                    }
                }
            });
        } catch (Exception ex) {
            Logger.getLogger(jsonImages.class.getName()).log(Level.SEVERE, null, ex);
        }
        // List of thumbs
        System.out.println("Checking images in " + properties.getProperty("imagesThumbDir"));
        fullPath = sContext.getRealPath("/") + properties.getProperty("imagesThumbDir");
        try {
            Stream<java.nio.file.Path> filePathStream = Files.walk(Paths.get(fullPath));
            filePathStream.forEach(filePath -> {
                if (Files.isRegularFile(filePath)) {
                    if (filePath.toString().contains(".lock")) {
                        String fileImage = filePath.toString().substring(0, filePath.toString().indexOf(".lock"));
                        File fI = new File(fileImage);
                        try {
                            System.out.println("Removing " + fileImage);
                            fI.delete();
                        } catch (Exception ex) {
                            //
                        }
                        try {
                            if (filePath.toFile().exists()) {
                                System.out.println("Removing " + filePath);
                                filePath.toFile().delete();
                            }
                        } catch (Exception ex) {
                            //
                        }
                    }
                }
            });
        } catch (Exception ex) {
            Logger.getLogger(jsonImages.class.getName()).log(Level.SEVERE, null, ex);
        }
        if (maxId > 0) {
        }
    }

    private void addRestResourceClasses(Set<Class<?>> resources) {
        resources.add(pro.elab.jsonImages.class);
    }

}
