#define NOMINMAX
#include "keystone/impl/Keystone.hpp"
#include "pugi4lunch/pugixml.hpp"
#include <curl/curl.h>

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
        
        // Just make sure that it ends with '/'
        if (this->url[this->url.size() - 1] != '/') {
            this->url.append(1, '/');
        }

    }


    /**
    * Logs the user in and returns a sessionToken
    */
    void  Keystone::login(const std::string& username, 
        const std::string& password,
        const std::string& tenantName,
        KeystoneUserInfo& info) {
            std::stringstream input;

            input << "<?xml version='1.0' encoding='UTF-8'?>" <<std::endl
                << "<auth xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' " << std::endl
                << "xmlns='http://docs.openstack.org/identity/api/v2.0' " <<std::endl
                << "tenantName='" << tenantName << "'>"<<std::endl
                << "<passwordCredentials username='" << username << "' password='" << password << "'/>"<<std::endl
                << "</auth>" << std::endl;

            std::stringstream output;

            std::string tokenUrl = url + "v2.0/tokens";
            write(tokenUrl, input, output);

	    pugi4lunch::pugi::xml_document document;
            if (!document.load(output)) {
                THROW("Could not parse xml document returned from server");
            }

            pugi4lunch::pugi::xml_node accessNode = document.child("access");

            if(!accessNode) {
                THROW("Unexpected XML document structure");
            }

            pugi4lunch::pugi::xml_node tokenNode = accessNode.child("token");

            if(!tokenNode) {
                THROW("Unexpected XML document structure");
            }

	    pugi4lunch::pugi::xml_attribute idAttribute = tokenNode.attribute("id");

            if(!idAttribute) {
                THROW("Unexpected XML document structure");
            }

            std::string id = idAttribute.as_string();
            if(id.size() == 0) {
                THROW("UnexpectedXML document structure");
            }
            info.setToken(id);

            info.setUsername(username);

	    pugi4lunch::pugi::xml_node userNode = accessNode.child("user");

            if (!userNode) {
                THROW("Unexpected reponse from server.");
            }
	    pugi4lunch::pugi::xpath_node_set roleNodes = userNode.select_nodes("role");

            std::vector<std::string> roles;

            for(pugi4lunch::pugi::xpath_node_set::const_iterator it = roleNodes.begin(); it != roleNodes.end(); ++it) {
                roles.push_back(it->node().attribute("name").as_string());
            }

            info.setRoles(roles);
    }


    /**
    * Gets the username of a sessionToken.
    * \throws runtime_error if the username could not be acquired (typically invalid sessiontoken)
    */
    void Keystone::getUserInfo(const std::string& tenantName, 
        const std::string& sessionToken, KeystoneUserInfo& info) {

            std::stringstream input;

            input << "<?xml version='1.0' encoding='UTF-8'?>" <<std::endl
                << "<auth xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' " << std::endl
                << "xmlns='http://docs.openstack.org/identity/api/v2.0' " <<std::endl
                << "tenantName='" << tenantName << "'>"<<std::endl
                << "<token id='" << sessionToken <<"' />"<<std::endl
                << "</auth>" << std::endl;

            std::stringstream output;

            std::string tokenUrl = url + "v2.0/tokens";
            write(tokenUrl, input, output);

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

    }

    void Keystone::write(const std::string& endpoint, std::stringstream& input, std::stringstream& output) {
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




        // Set headers:
        CurlListHolder headers;

        headers.list = curl_slist_append(headers.list, "Accept: application/xml");
        headers.list = curl_slist_append(headers.list, "Content-Type: application/xml");

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
