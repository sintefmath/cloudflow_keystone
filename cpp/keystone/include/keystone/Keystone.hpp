#pragma once
#include <string>

namespace keystone {
    
    class Keystone {
    public:
        /**
         * \param url the URL to the base of the keystone service.
         *            this is typically on the form "http://something.com/keystone"
         *            (note we omit the "v2.0" part here)
         */
        Keystone(const std::string& url);


        /**
         * Logs the user in and returns a sessionToken
         * \throws runtime_error if anything went wrong (http access, xml parsing, login error)
         */
        std::string login(const std::string& username, 
            const std::string& password,
            const std::string& tenantName);


        /**
         * Gets the username of a sessionToken.
         * \throws runtime_error if the username could not be acquired (typically invalid sessiontoken)
         */
        std::string getUsername(const std::string& tenantName, 
        const std::string& sessionToken);

    private:
        void write(const std::string& endpoint, std::stringstream& in, std::stringstream& out);
        std::string url;
    };

}