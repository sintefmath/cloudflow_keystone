#pragma once
#include <string>
#include "keystone/impl/KeystoneUserInfo.hpp"
#include <pugi4lunch/pugixml.hpp>


namespace keystone { namespace impl {
    class Keystone {
    public:
        /**
         * \param url the URL to the base of the keystone service.
         *            this is typically on the form "http://something.com/keystone"
         *            (note we omit the "v2.0" part here)
         */
        Keystone(const std::string& url);


        /**
         * Logs the user in and returns the userinfo
         * \throws runtime_error if anything went wrong (http access, xml parsing, login error)
         */
        void login(const std::string& username, 
            const std::string& password,
            const std::string& tenantName,
            KeystoneUserInfo& info);


        /**
         * Gets the userinfo of a sessionToken.
         * \throws runtime_error if the username could not be acquired (typically invalid sessiontoken)
         */
        void getUserInfo(const std::string& tenantName, 
            const std::string& sessionToken, KeystoneUserInfo& info);

        /**
         * Set the CA certification file name in order to correctly handle https urls
         * @param caCertFileName
         */
        void setCaCertFileName(const std::string& caCertFileName);


    private:
        void write(const std::string& endpoint,
                   const KeystoneUserInfo& info,
                   std::stringstream& in,
                   std::stringstream& out);

        void printXML(pugi4lunch::pugi::xml_node node, int intendation);
        void getRoles(const std::string &url, const std::string &sessionToken,
                                          KeystoneUserInfo& info);
        std::string url;
        std::string caCertFileName;
        bool userDefinedCaCertFile;

    };
 
}}
