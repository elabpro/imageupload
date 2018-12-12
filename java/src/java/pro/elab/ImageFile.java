/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
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
        Logger.getLogger(ImageFile.class.getName()).log(Level.INFO, "Trying to download " + url);
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
        Logger.getLogger(ImageFile.class.getName()).log(Level.INFO, "Trying to save content");
        DB db = new DB(properties.getProperty("redisHost"));
        int fileId = db.getCounter();
        String fileName = getFilename(fileId);
        try {
            byte[] decodedBytes = Base64.getDecoder().decode(content);
            writeFile(decodedBytes, fileName);
            createThumb(fileId);
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
        int fileId = db.getCounter();
        String fileName = getFilename(fileId);
        try {
            writeFile(bytes, fileName);
            createThumb(fileId);
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
        String fileName = getFilename(fileId);
        try {
            String lockName = properties.getProperty("path") + fileName + ".lock";
            File lockFile = new File(lockName);
            lockFile.createNewFile();
            byte[] buffer = new byte[is.available()];
            Files.copy(is, Paths.get(properties.getProperty("path") + fileName), StandardCopyOption.REPLACE_EXISTING);
            is.close();
            lockFile.delete();
            createThumb(fileId);
        } catch (Exception ex) {
            Logger.getLogger(ImageFile.class.getName()).log(Level.SEVERE, null, ex);
        }
        return fileId;
    }

    public int createThumb(int fileId) {
        String imageName = properties.getProperty("path") + getFilename(fileId, false);
        String thumbName = properties.getProperty("path") + getFilename(fileId, true);
        String lockName = thumbName + ".lock";
        File lockFile = new File(lockName);

        String size = "1024x768";
        ProcessBuilder pb = new ProcessBuilder("convert", imageName, "-resize","100x100!", thumbName);
        pb.redirectErrorStream(true);

        try {
            Process p = pb.start();
            BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line = null;
            while ((line = br.readLine()) != null) {
                //
            }
        } catch (Exception ex) {
            Logger.getLogger(ImageFile.class.getName()).log(Level.SEVERE, null, ex);
        }

        lockFile.delete();
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
        String lockName = fullpath + ".lock";
        File lockFile = new File(lockName);
        lockFile.createNewFile();
        Logger.getLogger(ImageFile.class.getName()).log(Level.INFO, "Saving file " + fullpath);
        File file = new File(fullpath);
        if (!file.exists()) {
            file.createNewFile();
        }
        FileOutputStream fop = new FileOutputStream(file);
        fop.write(content);
        fop.flush();
        fop.close();
        lockFile.delete();
    }

    /**
     * Generate filename from id
     *
     * @param fileId
     * @param thumb true - filename for thumb
     * @return
     */
    private String getFilename(int fileId, boolean thumb) {
        if (thumb) {
            return properties.getProperty("imagesThumbDir") + "/" + Integer.toString(fileId);
        } else {
            return properties.getProperty("imagesDir") + "/" + Integer.toString(fileId);
        }
    }

    /**
     * Generate filename from id
     *
     * @param fileId
     * @return
     */
    private String getFilename(int fileId) {
        return getFilename(fileId, false);
    }

}
