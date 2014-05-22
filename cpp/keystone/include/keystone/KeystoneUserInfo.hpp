#pragma once
#include <keystone/keystone.h>
#include <keystone/KeystoneSafeCall.hpp>
/**
 * \addtogroup keystone_wrapper
 * \{
 */
namespace keystone {
    class Keystone;

     
    /**
     * \ingroup keystone_wrapper
     * \brief A wrapper class to access the underlying C-interface
     * 
     * \note This class is header-only, meaning that it does not suffer from any definition problems of multiple compiler versions
     * 
     * \note It is non-copyable by design. If you wish to pass it around, either use references or (smart)-pointers.
     *
     * \note Use of this class still requires linking with the keystone C-library/DLL/so-file(Linux)
     **/
    class KeystoneUserInfo {
        friend class Keystone;
    public:
        /**
         * Constructs a new instance. 
         * \note Newly constructed \ref KeystoneUserInfo objects are not valid until they have been passed to a method 
         *       in \ref Keystone (eg. \ref Keystone::login og \ref Keystone::getUserInfoFromToken)
         *       calling any functions on a \ref KeystoneUserInfo -object before this will result in an exception
         */
        inline KeystoneUserInfo() : info(NULL) {}

        /**
         * Frees up the resources
         */
        ~KeystoneUserInfo() {
            if(info != NULL ) {
                KEYSTONE_SAFE_CALL(keystone_userinfo_free(info));
            }
        }

        /**
         * Gets the username
         * \sa KeystoneUserInfo::getUsername() const
         * 
         * \param[out] output will at end of execution have the username. 
         *                    (\c output will be resized to the actual username size)
         * 
         * \throws std::runtime_error if the object is in an invalid state (eg. if it has not been passed to either 
         *                            \ref Keystone::login or \ref Keystone::getUserInfoFromToken), or if something bad has happened.  
         */
        inline void getUsername(std::string& output) const {
            checkInfo();
            size_t usernameSize;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_username_buffer_size(info, &usernameSize));

            output.resize(usernameSize - 1);

            size_t dataWritten;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_username(info, &output[0], usernameSize, &dataWritten));
        }

        /**
         * Gets the username 
         * 
         * This is the more convenient version of  KeystoneUserInfo::getUsername(std::string&) const
         *
         * \sa KeystoneUserInfo::getUsername(std::string&) const
         *
         * \return The username associated to the userinfo
         * \throws std::runtime_error if the object is in an invalid state (eg. if it has not been passed to either 
         *                            \ref Keystone::login or \ref Keystone::getUserInfoFromToken), or if something bad has happened.  
         */
        inline std::string getUsername() const {
            std::string username;
            getUsername(username);
            return username;
        }

        /**
         * Gets the number of roles associated to the user. 
         * 
         * \sa KeystoneUserInfo::getRole
         *
         * \return The number of roles 
         * \throws std::runtime_error if the object is in an invalid state (eg. if it has not been passed to either 
         *                            \ref Keystone::login or \ref Keystone::getUserInfoFromToken), or if something bad has happened.  
         */
        inline size_t getRoleCount() const {
            checkInfo();
            size_t role_count;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_role_count(info, &role_count));
            return role_count;
        }

        /**
         * Gets the role at the given index
         * 
         * \sa KeystoneUserInfo::getRole(size_t) const
         * 
         * \param[in] index the index of the role to fetch 
         * 
         * \param[out] role will at end of execution have the role. 
         *                    (\c output will be resized to the actual role size)
         * 
         * \throws std::runtime_error if the object is in an invalid state (eg. if it has not been passed to either 
         *                            \ref Keystone::login or \ref Keystone::getUserInfoFromToken), or if something bad has happened.  
         */
        inline void getRole(size_t index, std::string& role) const {
            checkInfo();
            size_t roleBufferSize;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_role_buffer_size(info, index, &roleBufferSize));

            role.resize(roleBufferSize  - 1);

            size_t dataWritten;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_role(info, index, &role[0], roleBufferSize, &dataWritten));
        }


        /**
         * Gets the role at the given index
         * 
         * This is the more convenient version of \ref KeystoneUserInfo::getRole(size_t, std::string&) const
         *
         * \sa KeystoneUserInfo::getRole(size_t, std::string&) const
         *
         * \param[in] index the index of the role to fetch 
         *
         * \return The role at the given index
         * 
         * \throws std::runtime_error if the object is in an invalid state (eg. if it has not been passed to either 
         *                            \ref Keystone::login or \ref Keystone::getUserInfoFromToken), or if something bad has happened.  
         */
        inline std::string getRole(size_t index) const {
            std::string role;
            getRole(index, role);
            return role;
        }

        /**
         * Gets the token
         * \sa KeystoneUserInfo::getToken() const
         * 
         * \param[out] output will at end of execution have the token. 
         *                    (\c output will be resized to the actual token size)
         * 
         * \throws std::runtime_error if the object is in an invalid state (eg. if it has not been passed to either 
         *                            \ref Keystone::login or \ref Keystone::getUserInfoFromToken), or if something bad has happened.  
         */
        inline void getToken(std::string& output) const {
            checkInfo();
            size_t tokenSize;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_token_buffer_size(info, &tokenSize));

            output.resize(tokenSize - 1);

            size_t dataWritten;
            KEYSTONE_SAFE_CALL(keystone_userinfo_get_token(info, &output[0], tokenSize, &dataWritten));
        }


        /**
         * Gets the token 
         * 
         * This is the more convenient version of   \ref KeystoneUserInfo::getToken(std::string& output) const
         *
         * \sa KeystoneUserInfo::getToken(std::string&) const
         *
         * \return The token associated to the userinfo
         * \throws std::runtime_error if the object is in an invalid state (eg. if it has not been passed to either 
         *                            \ref Keystone::login or \ref Keystone::getUserInfoFromToken), or if something bad has happened.  
         */
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
/**
 * \}
 */