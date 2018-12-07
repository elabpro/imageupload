/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

/**
 *
 * @author eugine
 */
public class ImageFile {

    DB db;
    
    ImageFile(){
        db = new DB();
    }

    public int donwload(String url) {
        return 1;
    }

    public int saveContent(String content) {
        return 1;
    }

    public int saveFile(String filename) {
        return 1;
    }
}
