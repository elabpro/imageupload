/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import javax.ws.rs.core.Context;
import javax.ws.rs.core.UriInfo;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PUT;
import javax.ws.rs.core.MediaType;

/**
 * REST Web Service
 *
 * @author eugine
 */
@Path("images")
public class jsonImages {

    @Context
    private UriInfo context;

    /**
     * Creates a new instance of jsonImages
     */
    public jsonImages() {
    }

    /**
     * Retrieves representation of an instance of pro.elab.jsonImages
     *
     * @return an instance of java.lang.String
     */
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public String getJson() {
        //TODO return proper representation object
        return "[1]";
    }

    /**
     * POST method for updating or creating an instance of jsonImages
     *
     * @param content representation for the resource
     */
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    public String postJson(String content) {
        return "[done]";
    }
}
