/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
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
        db = new DB(properties.getProperty("redisHost"));
    }

    public int download(String url) {
        return 1;
    }

    public int saveContent(String content) {
        return 1;
    }

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
        File file = new File(filename);
        if (!file.exists()) {
            file.createNewFile();
        }
        FileOutputStream fop = new FileOutputStream(file);
        fop.write(content);
        fop.flush();
        fop.close();

    }
}
