#pragma once
#include <keystone/keystone.h>
#include <keystone/KeystoneSafeCall.hpp>

namespace keystone {
    class Keystone;

    class KeystoneUserInfo {
        friend class Keystone;
    public:
        inline KeystoneUserInfo() : info(NULL) {}
        ~KeystoneUserInfo() {
            if(info != NULL ) {
                KEYSTONE_SAFE_CALL(keystone_userinfo_free(info));
            }
        }

        inline void getUsername(std::string& output) const {
            checkInfo();
            size_t usernameSize;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_username_buffer_size(info, &usernameSize));

            output.resize(usernameSize - 1);

            size_t dataWritten;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_username(info, &output[0], usernameSize, &dataWritten));
        }

        inline std::string getUsername() const {
            std::string username;
            getUsername(username);
            return username;
        }

        inline size_t getRoleCount() const {
            checkInfo();
            size_t role_count;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_role_count(info, &role_count));
            return role_count;
        }

        inline void getRole(size_t index, std::string& role) const {
            checkInfo();
            size_t roleBufferSize;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_role_buffer_size(info, index, &roleBufferSize));

            role.resize(roleBufferSize  - 1);

            size_t dataWritten;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_role(info, index, &role[0], roleBufferSize, &dataWritten));
        }

        inline std::string getRole(size_t index) const {
            std::string role;
            getRole(index, role);
            return role;
        }

        inline void getToken(std::string& output) const {
            checkInfo();
            size_t tokenSize;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_token_buffer_size(info, &tokenSize));

            output.resize(tokenSize - 1);

            size_t dataWritten;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_token(info, &output[0], tokenSize, &dataWritten));
        }

        inline std::string getToken() const {
            std::string token;
            getToken(token);
            return token;
        }

        


    private:
        inline void setUserInfo(keystone_userinfo_t* info)  { 
            if (this->info != NULL) {
                KEYSTONE_SAFE_CALL(keystone_userinfo_free(this->info));
            }
            this->info = info; 
        }
        keystone_userinfo_t* info;

        // Disable copying
        KeystoneUserInfo(const KeystoneUserInfo& other) {}
        KeystoneUserInfo& operator=(const KeystoneUserInfo& other) {return *this; }

        inline void checkInfo() const {
            if(info == NULL) {
                throw std::runtime_error("Illegal state for KeystoneUserinfo");
            }
        }
    };
}