#pragma once

#include "keystone/keystone_export.h"

// For size_t: 
#include "stdlib.h"

struct keystone_data_struct;
typedef struct keystone_data_struct keystone_data_t;

struct keystone_userinfo_struct;
typedef struct keystone_userinfo_struct keystone_userinfo_t;

typedef enum {
    KEYSTONE_SUCCESS, KEYSTONE_UNKNOWN_ERROR  
} keystone_error_t;


KEYSTONE_EXPORT keystone_error_t keystone_init(const char* url, keystone_data_t** data);

KEYSTONE_EXPORT keystone_error_t keystone_free(keystone_data_t* data);

KEYSTONE_EXPORT keystone_error_t keystone_login(keystone_data_t* data, const char* username, const char* password, const char* tenant_name, keystone_userinfo_t** userinfo);

KEYSTONE_EXPORT keystone_error_t keystone_userinfo_free(keystone_userinfo_t* info);

KEYSTONE_EXPORT keystone_error_t keystone_get_userinfo_from_token(keystone_data_t* info, const char* tenant_name, const char* session_token, keystone_userinfo_t** userinfo);

KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_username(const keystone_userinfo_t* info, char* buffer, size_t buffer_length, size_t* data_written);

KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_username_buffer_size(const keystone_userinfo_t* info, size_t* size);

KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role_count(const keystone_userinfo_t* info, size_t* role_count);

KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role_buffer_size(const keystone_userinfo_t* info, size_t index, size_t* buffer_size);

KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role(const keystone_userinfo_t* info, size_t index, char* buffer, size_t buffer_len, size_t* data_written);

KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_token(const keystone_userinfo_t* info, char* buffer, size_t buffer_length, size_t* data_written);

KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_token_buffer_size(const keystone_userinfo_t* info, size_t* size);

