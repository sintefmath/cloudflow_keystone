#pragma once
#include <keystone/keystone.h>
#include <keystone/KeystoneUserInfo.hpp>
#include <keystone/KeystoneSafeCall.hpp>
#include <string>

/**
 * \addtogroup keystone_wrapper
 * \brief Meant as a wrapper C++ API for the C functions. Choose this API if you can use C++ and want to avoid messing around with pointers
 * \{
 */
namespace keystone {

    /**
     * \example keystone_check.cpp
     */

    /**
     * \example keystone_roles.cpp
     */

    
    /**
     * \example keystone_login.cpp
     */

    /**
     * \ingroup keystone_wrapper
     * 
     * \brief A wrapper class to access the underlying C-interface
     * 
     * \note This class is header-only, meaning that it does not suffer from any definition problems of multiple compiler versions
     * 
     * \note It is non-copyable by design. If you wish to pass it around, either use references or (smart)-pointers.
     *
     * \note Use of this class still requires linking with the keystone C-library/DLL/so-file(Linux)
     **/
    class Keystone {
    public:
        /**
         * Constructs a new keystone object. 
         * 
         * \param[in] url a string containing the URL for the keystone service. This is typically on the form
         *                http://server/keystone/ (that it is, without the "v2.0/"-part afterwards, it will be added).
         *                Also note that we require the trailing dash '/' at the end (http://server/keystone/ is valid, http://server/keystone is _not_)
         * 
         */
        Keystone(const std::string& url) {
            KEYSTONE_SAFE_CALL(keystone_init(url.c_str(), &data));
        }

        /**
         * Frees up the keystone resource
         */
        ~Keystone() {
            KEYSTONE_SAFE_CALL(keystone_free(data));
        }

        /**
         * Logs the user in and returns the associated userinfo. 
         * \sa keystone::KeystoneUserInfo
         *
         * \param[in] username the username 
         * \param[in] password the password
         * \param[in] tenantName the tenantName for the user (often just "users")
         * \param[out] info will at the end of execution contain the user information returned from keystone.
         * 
         * \throws std::runtime_error if an error occurred. Typically causes are network errors and login error (wrong username/password)
         */
        void login(const std::string& username, const std::string& password, const std::string& tenantName, KeystoneUserInfo& info) {
            keystone_userinfo_t* userInfo;
            KEYSTONE_SAFE_CALL(keystone_login(data, username.c_str(), password.c_str(), tenantName.c_str(), &userInfo));
            info.setUserInfo(userInfo);
        }

        /**
         * Gets the user information associated to a sessionToken (and throws an exception if it's an invalid sessionToken)
         * 
         * \sa keystone::KeystoneUserInfo
         * 
         * \param[in] tenantName the tenantName for the user (often just "users")
         * \param[in] sessionToken the sessionToken to use. 
         * \param[out] info will at the end of execution contain the user information returned from keystone.
         * 
         * \throws std::runtime_error if an error occurred. Typically causes are network errors and authentication errors (wrong sessionToken)
         */
        void getUserInfoFromToken(const std::string& tenantName, const std::string& sessionToken, KeystoneUserInfo& info) {
            keystone_userinfo_t* userInfo;
            KEYSTONE_SAFE_CALL(keystone_get_userinfo_from_token(data, tenantName.c_str(), sessionToken.c_str(), &userInfo));
            info.setUserInfo(userInfo);
        }

	
        void setCACertificateFilename(const std::string& certFileName, KeystoneUserInfo& info) {
            //keystone_userinfo_t* userInfo;
            //KEYSTONE_SAFE_CALL(keystone_set_ca_certificate_filename(certFileName, info));
            //info.setUserInfo(userInfo);
        }
	

    private: 
        keystone_data_t* data;

        // We do not want to be able to copy this:
        Keystone(const Keystone& other) {}
        Keystone& operator=(const Keystone& other) { return *this; }
    };
}
/**
 * \}
 */
