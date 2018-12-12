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
        Logger.getLogger(ImageFile.class.getName()).log(Level.INFO, null, "Trying to download " + url);
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
        Logger.getLogger(ImageFile.class.getName()).log(Level.INFO, null, "Trying to save content");
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
     * Record content in the file
     *
     * @param bytes file content
     * @return file id
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
     * Record stream in the file
     *
     * @param is InputStream
     * @return file id
     */
    public int saveFile(InputStream is) {
        DB db = new DB(properties.getProperty("redisHost"));
        int fileId = db.getCounter();
        String fileName = properties.getProperty("imagesDir") + "/" + Integer.toString(fileId);
        try {
            byte[] buffer = new byte[is.available()];
            is.read(buffer);
            writeFile(buffer, fileName);
            is.close();
        } catch (IOException ex) {
            Logger.getLogger(ImageFile.class.getName()).log(Level.SEVERE, null, ex);
        }
        return fileId;
    }

    public int createThumb(int fileId) {
        return 1;
    }

    /**
     * Save content in the file
     *
     * @param content content
     * @param filename filename
     * @throws IOException
     */
    private void writeFile(byte[] content, String filename) throws IOException {
        String fullpath = properties.getProperty("path") + filename;
        Logger.getLogger(ImageFile.class.getName()).log(Level.INFO, null, "Saving file " + fullpath);
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
