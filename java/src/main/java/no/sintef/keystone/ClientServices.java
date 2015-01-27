/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package no.sintef.keystone;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.math.BigDecimal;
import java.net.HttpURLConnection;
import java.net.URL;
import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonObject;
import javax.json.JsonReader;
import javax.json.JsonValue;
import javax.json.JsonWriter;
import javax.net.ssl.HttpsURLConnection;

/**
 *
 * @author kjetilo
 */
public class ClientServices {
    static {
       	System.setProperty("jsse.enableSNIExtension", "false");
    }
    final private URL url;
    
    public ClientServices(URL url) {
        this.url = url;
    }
    
    /**
     * Logs the user in and returns a sessiontoken.
     * @param username the username
     * @param password the password to use.
     * @note Defaults to the 
     * @return 
     */
    public String login(String username, String password, String tenant) throws IOException {
        
        
        HttpsURLConnection connection = (HttpsURLConnection)(new URL(url, "v2.0/tokens")).openConnection();
        JsonObject inputObject = Json.createObjectBuilder()
                .add("auth", Json.createObjectBuilder()
                    .add("passwordCredentials", Json.createObjectBuilder()
                        .add("username", username)
                        .add("password", password))
                .add("tenantName", tenant)).build();
        
        connection.setRequestProperty("Content-Type", "application/json");
        connection.setRequestProperty("Accept", "application/json");
        
        connection.setDoInput(true);
        connection.setDoOutput(true);
        
        connection.setRequestMethod("POST");
        
        OutputStream output = connection.getOutputStream();
        
        
        JsonWriter writer = Json.createWriter(output);
        writer.write(inputObject);
        writer.close();
        output.close();
        
        InputStream input = connection.getInputStream();
        
        JsonReader reader = Json.createReader(input);
        
        JsonObject outputObject = reader.readObject();
        reader.close();
        input.close();
        
        return outputObject.getJsonObject("access").getJsonObject("token").getString("id");
    }
    
    /**
     * Checks if the given user is logged in with the given token.
     * @param userToken the token
     * @return the userName of the current user
     */
    public String getUsername(String userToken, String tenant) throws IOException {
       HttpsURLConnection connection = (HttpsURLConnection)(new URL(url, "v2.0/tokens").openConnection());
        
       connection.setRequestProperty("Accept", "application/json");
        
        connection.setDoInput(true);
        connection.setDoOutput(true);
        
        connection.setRequestMethod("POST");
        JsonObject inputObject = Json.createObjectBuilder()
                .add("auth", Json.createObjectBuilder()
                    .add("token", Json.createObjectBuilder()
                        .add("id", userToken))
                .add("tenantName", tenant)).build();
        
        connection.setRequestProperty("Content-Type", "application/json");
        
        OutputStream output = connection.getOutputStream();
        JsonWriter writer = Json.createWriter(output);
        writer.write(inputObject);
        writer.close();
        output.close();
        
        int response = connection.getResponseCode();
        
        if(response == 401) {
            throw new RuntimeException("Bad authorization: userToken = " + userToken + ", tenant = " + tenant);
        }
        
        if(response != 203 && response != 200) {
            throw new RuntimeException("Could not get username");
        }
        InputStream input = connection.getInputStream();
        JsonReader reader = Json.createReader(input);
        
        JsonObject outputObject = reader.readObject();
        reader.close();
        input.close();
        
        return outputObject.getJsonObject("access").getJsonObject("user").getString("username");
                
    }
}
