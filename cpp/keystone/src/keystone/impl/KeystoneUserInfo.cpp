#include "keystone/impl/KeystoneUserInfo.hpp"

namespace keystone { namespace impl {



    KeystoneUserInfo::KeystoneUserInfo()
    {

    }

    const std::string& KeystoneUserInfo::getUsername() const
    {
        return username;
    }

    void KeystoneUserInfo::setUsername( const std::string& username )
    {
        this->username = username;
    }

    const std::vector<std::string>& KeystoneUserInfo::getRoles() const
    {
        return roles;
    }

    void KeystoneUserInfo::setRoles( const std::vector<std::string>& roles )
    {
        this->roles = roles;
    }

    void KeystoneUserInfo::setToken( const std::string& token )
    {
        this->token = token;
    }

    const std::string& KeystoneUserInfo::getToken() const
    {
        return token;
    }

}}


