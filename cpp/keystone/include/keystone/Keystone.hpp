#pragma once
#include <keystone/keystone.h>
#include <keystone/KeystoneUserInfo.hpp>
#include <keystone/KeystoneSafeCall.hpp>
#include <string>


namespace keystone {
    /**
     * $end
     **/
    class Keystone {
    public:

        Keystone(const std::string& url) {
            KEYSTONE_SAFE_CALL(keystone_init(url.c_str(), &data));
        }

        ~Keystone() {
            KEYSTONE_SAFE_CALL(keystone_free(data));
        }

        void login(const std::string& username, const std::string& password, const std::string& tenantName, KeystoneUserInfo& info) {
            keystone_userinfo_t* userInfo;
            KEYSTONE_SAFE_CALL(keystone_login(data, username.c_str(), password.c_str(), tenantName.c_str(), &userInfo));
            info.setUserInfo(userInfo);
        }

        void getUserInfoFromToken(const std::string& tenantName, const std::string& sessionToken, KeystoneUserInfo& info) {
            keystone_userinfo_t* userInfo;
            KEYSTONE_SAFE_CALL(keystone_get_userinfo_from_token(data, tenantName.c_str(), sessionToken.c_str(), &userInfo));
            info.setUserInfo(userInfo);
        }

    private: 
        keystone_data_t* data;

        // We do not want to be able to copy this:
        Keystone(const Keystone& other) {}
        Keystone& operator=(const Keystone& other) { return *this; }
    };
}