#define NOMINMAX
#include "keystone/impl/Keystone.hpp"
#include "pugi4lunch/pugixml.hpp"
#include <curl/curl.h>

#include <stdlib.h>
#include <stdexcept>
#include <sstream>
#include <numeric>
#include <iostream>


#define THROW(msg) { \
    std::stringstream ss; \
    ss<< "At " << __FILE__ << "(" << __LINE__ << "): \"" << msg << "\""; \
    throw std::runtime_error(ss.str()); \
} 


#define KEYSTONE_CURL_SAFE_CALL(x) {\
    CURLcode result = x; \
    if ( result != CURLE_OK) { \
    THROW("Curl error code: " << result); \
    } \
} 

namespace {
    size_t writeToSS(char* dataPointer, size_t size, size_t nmemb, void* stringStreamAsVoid) {

        std::stringstream* ss = static_cast<std::stringstream*>(stringStreamAsVoid);

        ss->write(dataPointer, size * nmemb);
        return size * nmemb;
    }

    size_t readFromSS(void *ptr, size_t size, size_t nmemb, void *stringStreamAsVoid) {
        std::stringstream* ss = static_cast<std::stringstream*>(stringStreamAsVoid);
        size_t beginIndex = ss->tellg();
        ss->seekg(0, ss->end);
        size_t currentSize = (size_t)ss->tellg() - beginIndex;
        ss->seekg(beginIndex);

        size_t sizeToRead = std::min(currentSize, size * nmemb);

        ss->read((char*) ptr, sizeToRead);
        return sizeToRead;
    }

    // In lack of unique-pointers:
    struct CurlHolder {
        CURL* curl;
        CurlHolder(CURL* curl_) {
            curl = curl_;
        }
        ~CurlHolder() {
            if(curl != NULL) {
                curl_easy_cleanup(curl);
            }
        }
    };

    // In lack of unique-pointers:
    struct CurlListHolder {
        struct curl_slist* list;
        CurlListHolder() {
            list = NULL;
        }
        ~CurlListHolder() {
            if(list != NULL) {
                curl_slist_free_all(list);
            }
        }
    };

}


namespace keystone { namespace impl {
    /**
    * \param url the URL to the base of the keystone service.
    *            this is typically on the form "http://something.com/keystone"
    *            (note we omit the "v2.0" part here)
    */
    Keystone::Keystone(const std::string& url) {
        if(url.size() == 0) {
            THROW("Illegal length of URL");
        }
        this->url = url;
        
        // TODO: Check that url is correct. Should end with "?wsdl"

        userDefinedCaCertFile = false;

    }


    /**
    * Logs the user in and returns a sessionToken
    */
    void  Keystone::login(const std::string& username, 
        const std::string& password,
        const std::string& tenantName,
        KeystoneUserInfo& info) {

            std::stringstream inputXML;

            inputXML << "<?xml version='1.0' encoding='UTF-8'?>\n"
                     << "<S:Envelope xmlns:S='http://schemas.xmlsoap.org/soap/envelope/' xmlns:SOAP-ENV='http://schemas.xmlsoap.org/soap/envelope/'>\n"
                     << "<SOAP-ENV:Header/>\n"
                     << "<S:Body>\n"
                     << "<ns2:getSessionToken xmlns:ns2='http://authmanager.sintef.no/'>\n"
                     << "<ns2:username>" << username << "</ns2:username>\n"
                     << "<ns2:password>"<< password << "</ns2:password>\n"
                     << "<ns2:project>" << tenantName << "</ns2:project>\n"
                     << "</ns2:getSessionToken>\n"
                     <<"</S:Body>\n"
                     << "</S:Envelope>\n";

            std::stringstream output;
            write(url, info, inputXML, output);

            pugi4lunch::pugi::xml_document document;
            if (!document.load(output)) {
                THROW("Could not parse xml document returned from server");
            }

            //printXML(document.root(), 0);

            pugi4lunch::pugi::xml_node envelopeNode = document.child("S:Envelope");

            if(!envelopeNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node bodyNode = envelopeNode.child("S:Body");

            if(!bodyNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node responseNode= bodyNode.child("ns2:getSessionTokenResponse");

            if(!responseNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node returnNode = responseNode.child("return");
            if (!returnNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node tokenNode = returnNode.first_child();
            if (!tokenNode) {
                THROW("Unexpected XML document structure");
            }

            std::string id = tokenNode.value();
            if(id.size() == 0) {
                THROW("UnexpectedXML document structure");
            }
            info.setToken(id);

            info.setUsername(username);

        /*pugi4lunch::pugi::xml_node userNode = envelopeNode.child("user");

            if (!userNode) {
                THROW("Unexpected reponse from server.");
            }
	    pugi4lunch::pugi::xpath_node_set roleNodes = userNode.select_nodes("role");

            std::vector<std::string> roles;

            for(pugi4lunch::pugi::xpath_node_set::const_iterator it = roleNodes.begin(); it != roleNodes.end(); ++it) {
                roles.push_back(it->node().attribute("name").as_string());
            }

            info.setRoles(roles);*/
    }


    void Keystone::printXML(pugi4lunch::pugi::xml_node node, int intendation) {
        std::string space = "";
        for (int i = 0; i < intendation; i++) {
            space = space + "  ";
        }
        std::cout << space << "NODE" << std::endl;
        std::cout << space << "name: " << node.name() << " - value: " << node.value() << std::endl;
        space = space + "  ";
        std::cout << space << "ATTRIBUTES" << std::endl;
        for (pugi4lunch::pugi::xml_attribute a = node.first_attribute(); a; a = a.next_attribute()) {
            std::cout << space << "name: " << a.name() << " - value: " << a.as_string() << std::endl;
        }
        for (pugi4lunch::pugi::xml_node n = node.first_child(); n; n = n.next_sibling()) {
            printXML(n, intendation +1);
        }
    }


    /**
    * Gets the username of a sessionToken.
    * \throws runtime_error if the username could not be acquired (typically invalid sessiontoken)
    */
    void Keystone::getUserInfo(const std::string& tenantName, 
        const std::string& sessionToken, KeystoneUserInfo& info) {

            std::stringstream inputXML;

            inputXML << "<?xml version='1.0' encoding='UTF-8'?>\n"
                     << "<S:Envelope xmlns:S='http://schemas.xmlsoap.org/soap/envelope/' xmlns:SOAP-ENV='http://schemas.xmlsoap.org/soap/envelope/'>\n"
                     << "<SOAP-ENV:Header/>\n"
                     << "<S:Body>\n"
                     << "<ns2:getUsername xmlns:ns2='http://authmanager.sintef.no/'>\n"
                     << "<ns2:sessionToken>" << sessionToken << "</ns2:sessionToken>\n"
                     << "</ns2:getUsername>\n"
                     <<"</S:Body>\n"
                     << "</S:Envelope>\n";


            std::stringstream output;

            std::string tokenUrl = url;
            write(tokenUrl, info, inputXML, output);

            pugi4lunch::pugi::xml_document document;
            if (!document.load(output)) {
                THROW("Could not parse xml document returned from server");
            }

            //printXML(document.root(), 0);

            pugi4lunch::pugi::xml_node envelopeNode = document.child("S:Envelope");

            if(!envelopeNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node bodyNode = envelopeNode.child("S:Body");

            if(!bodyNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node responseNode= bodyNode.child("ns2:getUsernameResponse");

            if(!responseNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node returnNode = responseNode.child("return");
            if (!returnNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node usernameNode = returnNode.first_child();
            if (!usernameNode) {
                THROW("Unexpected XML document structure");
            }

            std::string username = usernameNode.value();
            if(username.size() == 0) {
                THROW("UnexpectedXML document structure");
            }
            info.setToken(sessionToken);

            info.setUsername(username);



/**
            pugi4lunch::pugi::xml_document document;
            if (!document.load(output)) {
                THROW("Could not parse xml document returned from server");
            }

	    pugi4lunch::pugi::xml_node accessNode = document.child("access");

            if(!accessNode) {
                THROW("Unexpected XML document structure");
            }

	    pugi4lunch::pugi::xml_node userNode = accessNode.child("user");

            if(!userNode) {
                THROW("Unexpected XML document structure");
            }

	    pugi4lunch::pugi::xml_attribute usernameAttribute = userNode.attribute("username");
            if(!usernameAttribute) {
                THROW("Unexpected XML document structure");
            }

            info.setUsername(usernameAttribute.as_string());
            
	    pugi4lunch::pugi::xpath_node_set roleNodes = userNode.select_nodes("role");

            std::vector<std::string> roles;

            for(pugi4lunch::pugi::xpath_node_set::const_iterator it = roleNodes.begin(); it != roleNodes.end(); ++it) {
                roles.push_back(it->node().attribute("name").as_string());
            }

            info.setRoles(roles);

            info.setToken(sessionToken);
*/
    }


    void Keystone::setCaCertFileName(const std::string &caCertFileName) {
        this->caCertFileName = caCertFileName;
        userDefinedCaCertFile = true;
    }

    void Keystone::write(const std::string& endpoint, const KeystoneUserInfo& info,
                         std::stringstream& input, std::stringstream& output) {
        CurlHolder curl(curl_easy_init());
        if (!curl.curl) {
            THROW("Could not initialize curl");
        }

        curl_easy_setopt(curl.curl, CURLOPT_URL, endpoint.c_str());

        curl_easy_setopt(curl.curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl.curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl.curl, CURLOPT_WRITEFUNCTION, writeToSS);
        curl_easy_setopt(curl.curl, CURLOPT_WRITEDATA, &output);

        // Get size:
        input.seekg(0, input.end);
        size_t inputSize = input.tellg();
        input.seekg(0, input.beg);
        curl_easy_setopt(curl.curl, CURLOPT_POSTFIELDSIZE, inputSize);

        curl_easy_setopt(curl.curl, CURLOPT_READFUNCTION, readFromSS);
        curl_easy_setopt(curl.curl, CURLOPT_READDATA, &input);

        // Check environmental variable or if the user has provided certification
        // file name:
        char* envCaCertFileName;
        envCaCertFileName = getenv("KEYSTONE_SET_CA_CERTIFICATE_FILENAME");
        if (this->userDefinedCaCertFile) {
            curl_easy_setopt(curl.curl, CURLOPT_CAINFO, this->caCertFileName.c_str());
        }
        else if ( envCaCertFileName != NULL ) {
            curl_easy_setopt(curl.curl, CURLOPT_CAINFO, envCaCertFileName);
        }


        // Set headers:
        CurlListHolder headers;

        headers.list = curl_slist_append(headers.list, "Accept: text/xml");
        headers.list = curl_slist_append(headers.list, "Content-Type: text/xml");

        curl_easy_setopt(curl.curl, CURLOPT_HTTPHEADER, headers.list);
        KEYSTONE_CURL_SAFE_CALL(curl_easy_perform(curl.curl));


        long returnCode;
        KEYSTONE_CURL_SAFE_CALL(curl_easy_getinfo(curl.curl, CURLINFO_RESPONSE_CODE, &returnCode));

        if (returnCode != 200 && returnCode != 203) {
            std::stringstream ss;

            THROW("Unexpected returncode: " << + returnCode);
        }
    }
}
}
