#pragma once
#include <string>
#include <vector>

namespace keystone {
    namespace impl {
        class KeystoneUserInfo {
        public:
            KeystoneUserInfo();

            const std::string& getUsername() const;
            void setUsername(const std::string& username);

            const std::vector<std::string>& getRoles() const;

            void setRoles(const std::vector<std::string>& roles);

            void setToken(const std::string& token);

            const std::string& getToken() const;

        private:
            std::string username;
            std::vector<std::string> roles;
            std::string token;
        };
    }
}
