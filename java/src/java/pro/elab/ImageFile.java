/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.Base64;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletContext;
import javax.ws.rs.core.Context;

/**
 *
 * @author eugine
 */
public class ImageFile {

    DB db;
    Properties properties;

    ImageFile(Properties p) {
        properties = p;
        db = new DB(properties.getProperty("redisHost").toString());
    }

    public int download(String url) {
        System.out.println("Trying to download " + url);
        int fileId = 0;
        try {
            InputStream in = new URL(url).openStream();
            fileId = saveFile(in);
        } catch (Exception ex) {
            Logger.getLogger(ImageFile.class.getName()).log(Level.SEVERE, null, ex);
        }
        return fileId;
    }

    public int saveContent(String content) {
        System.out.println("Trying to save content");
        DB db = new DB(properties.getProperty("redisHost"));
        int fileId = db.getCounter();
        String fileName = properties.getProperty("imagesDir") + "/" + Integer.toString(fileId);
        try {
            byte[] decodedBytes = Base64.getDecoder().decode(content);
            writeFile(decodedBytes, fileName);
        } catch (Exception ex) {
            Logger.getLogger(ImageFile.class.getName()).log(Level.SEVERE, null, ex);
        }
        return fileId;
    }

    /**
     * Запись буфера в файл
     *
     * @param bytes
     * @return id файла
     */
    public int saveFile(byte[] bytes) {
        DB db = new DB(properties.getProperty("redisHost"));
        String fileName = properties.getProperty("imagesDir") + "/" + Integer.toString(db.getCounter());
        try {
            writeFile(bytes, fileName);
        } catch (IOException ex) {
            Logger.getLogger(ImageFile.class.getName()).log(Level.SEVERE, null, ex);
        }
        return 1;
    }

    /**
     * Запись потока в файл
     *
     * @param is
     * @return id файла
     */
    public int saveFile(InputStream is) {
        DB db = new DB(properties.getProperty("redisHost"));
        int fileId = db.getCounter();
        String fileName = properties.getProperty("imagesDir") + "/" + Integer.toString(fileId);
        try {
            byte[] buffer = new byte[is.available()];
            is.read(buffer);
            writeFile(buffer, fileName);
        } catch (IOException ex) {
            Logger.getLogger(ImageFile.class.getName()).log(Level.SEVERE, null, ex);
        }
        return fileId;
    }

    public int createThumb(int fileId) {
        return 1;
    }

    /**
     * Save base64 content in to the file
     *
     * @param content
     * @param filename
     * @throws IOException
     */
    private void writeFile(byte[] content, String filename) throws IOException {
        String fullpath = properties.getProperty("path") + filename;
        System.out.println("Saving file " + fullpath);
        File file = new File(fullpath);
        if (!file.exists()) {
            file.createNewFile();
        }
        FileOutputStream fop = new FileOutputStream(file);
        fop.write(content);
        fop.flush();
        fop.close();
    }
}
