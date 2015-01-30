#include "keystone/keystone.h"
#include "keystone/impl/Keystone.hpp"
#include <iostream>

#define KEYSTONE_METHOD_START try {

#define KEYSTONE_METHOD_END return KEYSTONE_SUCCESS; } catch(...) { return KEYSTONE_UNKNOWN_ERROR; }
struct keystone_data_struct {
    keystone::impl::Keystone* impl;
};

struct keystone_userinfo_struct {
    keystone::impl::KeystoneUserInfo* impl;
};
extern "C" {


keystone_error_t keystone_init(const char* url, keystone_data_t** data) {
    KEYSTONE_METHOD_START
        *data = new keystone_data_t();
        (*data)->impl = new keystone::impl::Keystone(url);
        
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_free(keystone_data_t* data) {
    KEYSTONE_METHOD_START
        delete data->impl;
        delete data;

   KEYSTONE_METHOD_END
}

keystone_error_t keystone_login(keystone_data_t* data, const char* username, const char* password, const char* tenant_name, keystone_userinfo_t** userinfo) {
    KEYSTONE_METHOD_START
	try {
	    // Give sane default values: 
	    *userinfo = NULL;
	    *userinfo = new keystone_userinfo_t();
	    (*userinfo)->impl = NULL;

	    (*userinfo)->impl = new keystone::impl::KeystoneUserInfo();

	    data->impl->login(username, password, tenant_name, *((*userinfo)->impl));
	} catch(...) {
	    // Free up data: 
	    if (*userinfo != NULL) {
		if ((*userinfo)->impl != NULL) {
		    delete (*userinfo)->impl;
		    (*userinfo)->impl = NULL;
		}
		delete *userinfo;
		*userinfo = NULL;
	    }
	    return KEYSTONE_UNKNOWN_ERROR;
	}

    KEYSTONE_METHOD_END
}

keystone_error_t keystone_userinfo_free(keystone_userinfo_t* info) {
    KEYSTONE_METHOD_START
	bool some_argument_null = !((info != NULL) && (info->impl != NULL));
	if ( info != NULL) {
	    if ( info->impl != NULL) {
		delete info->impl;
		info->impl = NULL;
	    }
	    delete info;
	    info = NULL;
	}
	if (some_argument_null) {
	    return KEYSTONE_UNKNOWN_ERROR;
	}
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_get_userinfo_from_token(keystone_data_t* data, const char* tenant_name, const char* session_token, keystone_userinfo_t** userinfo) {
    KEYSTONE_METHOD_START
       try {
	    // Give sane default values: 
	    *userinfo = NULL;
	    *userinfo = new keystone_userinfo_t();
	    (*userinfo)->impl = NULL;

	    (*userinfo)->impl = new keystone::impl::KeystoneUserInfo();

	    data->impl->getUserInfo(tenant_name, session_token, *((*userinfo)->impl));
	} catch(...) {
	    // Free up data: 
	    if (*userinfo != NULL) {
		if ((*userinfo)->impl != NULL) {
		    delete (*userinfo)->impl;
		    (*userinfo)->impl = NULL;
		}
		delete *userinfo;
		*userinfo = NULL;
	    }
	    return KEYSTONE_UNKNOWN_ERROR;
	}
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_set_ca_certificate_filename(keystone_data_t* data, const char* cert_file_name) {
    KEYSTONE_METHOD_START
    data->impl->setCaCertFileName(cert_file_name);
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_userinfo_get_username(const keystone_userinfo_t* info, char* buffer, size_t buffer_length, size_t* data_written) {
    KEYSTONE_METHOD_START
        size_t size_to_write;
        keystone_error_t size_error = keystone_userinfo_get_username_buffer_size(info, &size_to_write);
        if (size_error !=  KEYSTONE_SUCCESS) {
            return size_error;
        }

        if (buffer_length < size_to_write) {
            return KEYSTONE_UNKNOWN_ERROR;
        }

        if( size_to_write == 0 ) {
            return KEYSTONE_UNKNOWN_ERROR;
        }

        info->impl->getUsername().copy(buffer, size_to_write - 1);

        // We need to null terminate (copy does not include it)
        buffer[size_to_write - 1] = '\0';

        *data_written = size_to_write;
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_userinfo_get_username_buffer_size(const keystone_userinfo_t* info, size_t* size) {
    KEYSTONE_METHOD_START
        *size = info->impl->getUsername().size() + 1;
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_userinfo_get_role_count(const keystone_userinfo_t* info, size_t* role_count) {
    KEYSTONE_METHOD_START
        *role_count = info->impl->getRoles().size();
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_userinfo_get_role_buffer_size(const keystone_userinfo_t* info, size_t index, size_t* buffer_size) {
    KEYSTONE_METHOD_START
        if (index >= info->impl->getRoles().size()) {
            return KEYSTONE_UNKNOWN_ERROR;
        }
        *buffer_size = info->impl->getRoles()[index].size() + 1;
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_userinfo_get_role(const keystone_userinfo_t* info, size_t index, char* buffer, size_t buffer_len, size_t* data_written) {
    KEYSTONE_METHOD_START
        if (index >= info->impl->getRoles().size()) {
            return KEYSTONE_UNKNOWN_ERROR;
        }
        size_t size_to_write;

        keystone_error_t size_error = keystone_userinfo_get_role_buffer_size(info, index, &size_to_write);

        if ( size_error != KEYSTONE_SUCCESS) {
            return size_error;
        }

        if ( buffer_len < size_to_write ) {
            return KEYSTONE_UNKNOWN_ERROR;
        }

        info->impl->getRoles()[index].copy(buffer, size_to_write - 1);
        buffer[size_to_write - 1] = '\0';

    KEYSTONE_METHOD_END
}


keystone_error_t keystone_userinfo_get_token(const keystone_userinfo_t* info, char* buffer, size_t buffer_length, size_t* data_written) {
    KEYSTONE_METHOD_START
        size_t size_to_write;
        keystone_error_t size_error = keystone_userinfo_get_token_buffer_size(info, &size_to_write);
        if (size_error !=  KEYSTONE_SUCCESS) {
            return size_error;
        }

        if (buffer_length < size_to_write) {
            return KEYSTONE_UNKNOWN_ERROR;
        }

        if( size_to_write == 0 ) {
            return KEYSTONE_UNKNOWN_ERROR;
        }

        info->impl->getToken().copy(buffer, size_to_write - 1);

        // We need to null terminate (copy does not include it)
        buffer[size_to_write - 1] = '\0';

        *data_written = size_to_write;
    KEYSTONE_METHOD_END
}

keystone_error_t keystone_userinfo_get_token_buffer_size(const keystone_userinfo_t* info, size_t* size) {
    KEYSTONE_METHOD_START
        *size = info->impl->getToken().size() + 1;
    KEYSTONE_METHOD_END
}
}
