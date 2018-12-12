/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import java.io.InputStream;
import java.util.Base64;
import java.util.Properties;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author eugine
 */
public class ImageFileTest {

        ImageFile instance;
    
    public ImageFileTest() {
        Properties p;
        p = new Properties();
        p.setProperty("imagesDir", "/tmp");
        p.setProperty("imagesThumbDir", "/tmp");
        p.setProperty("redisHost", "localhost");
        p.setProperty("path","");
        instance = new ImageFile(p);
    }

    /**
     * Test of saveContent method, of class ImageFile.
     */
    @Test
    public void testSaveContent() {
        System.out.println("saveContent");
        String content = Base64.getEncoder().encodeToString("Test".getBytes());
        int expResult = 0;
        int result = instance.saveContent(content);
        assertTrue(expResult < result);
    }

    /**
     * Test of saveFile method, of class ImageFile.
     */
    @Test
    public void testSaveFile_byteArr() {
        System.out.println("saveFile");
        byte[] bytes = null;
        ImageFile instance = null;
        int expResult = 0;
//        int result = instance.saveFile(bytes);
//        assertEquals(expResult, result);
    }

    /**
     * Test of saveFile method, of class ImageFile.
     */
    @Test
    public void testSaveFile_InputStream() {
        System.out.println("saveFile");
        InputStream is = null;
        ImageFile instance = null;
        int expResult = 0;
//        int result = instance.saveFile(is);
//        assertEquals(expResult, result);
    }

    /**
     * Test of createThumb method, of class ImageFile.
     */
    @Test
    public void testCreateThumb() {
        System.out.println("createThumb");
        int fileId = 1;
        int expResult = 1;
        int result = instance.createThumb(fileId);
        assertEquals(expResult, result);
    }
    
}
