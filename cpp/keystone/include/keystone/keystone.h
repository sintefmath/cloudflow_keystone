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
#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Creates a new instance of the keystone object. All further calls expects an instance created from this function.
     * 
     * \param[in] url a (null terminated) string containing the URL for the keystone service. This is typically on the form
     *                http://server/keystone/ (that it is, without the "v2.0/"-part afterwards, it will be added).
     * 
     * \param[out] handle a pointer to a pointer to a data. At the end of a successful run, this will contain a valid pointer to a keystone_handle
     * 
     * \return GSS_SUCCESS if everything went OK, something else if there was an error.
     *
     * \note All data objects initialized with keystone_init _must_ be freed with keystone_free (ordinary free will not work!)
     *
     * <example>
     * keystone_data_t* keystone_handle;
     * keystone_error_t keystone_init_error = keystone_init("http://server.com/keystone/", &keystone_handle);
     * 
     * if ( keystone_init_error != GSS_SUCCESS ) {
     *     // Something went wrong
     * }
     * 
     * // use it...
     * 
     * // remember to free it at the end:
     * keystone_error_t keystone_free_error = keystone_free(keystone_handle);
     * 
     * if ( keystone_free_error != GSS_SUCCESS ) {
     *     // Something went wrong...
     * }
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_init(const char* url, keystone_data_t** handle);


    /**
     * Frees up all resources associated to the handle.
     * \note This is _not_ equivalent to calling ordinary free on the object. It is not possible to call free on the handle!
     * 
     * \param[in] handle a pointer to a valid keystone_handle obtained from \sa{keystone_init}
     * \return GSS_SUCCESS if everything went OK, something else otherwise.
     */
    KEYSTONE_EXPORT keystone_error_t keystone_free(keystone_data_t* handle);


    /**
     * Logs the user in and gives back a session token. 
     * 
     * \param[in] handle a valid handle to keystone (obtained by \sa keystone_init).
     * 
     * \param[in] username a null terminated string containing the username
     *
     * \param[in] password a null terminated string containing the password
     * 
     * \param[in] tenant_name a null terminated string containing the tenant_name
     * 
     * \param[out] userinfo at end of execution, will contain a valid handle to a userinfo object. 
     * 
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     * 
     * \note All userinfo_objects must be freed with \sa keystone_userinfo_free
     *
     *  <example>
     * // assume keystone_handle is initialized
     * keystone_userinfo_t* userinfo_handle;
     * keystone_error_t keystone_login_error = keystone_login(keystone_handle, "username", "password", "tenant_name", &userinfo_handle);
     * 
     * if ( keystone_login_error != GSS_SUCCESS ) {
     *     // Something went wrong
     * }
     * 
     * // use it...
     * 
     * // remember to free it at the end:
     * keystone_error_t keystone_userinfo_free_error = keystone_userinfo_free(userinfo_handle);
     * 
     * if ( keystone_userinfo_free_error != GSS_SUCCESS ) {
     *     // Something went wrong...
     * }
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_login(keystone_data_t* handle, const char* username, const char* password, const char* tenant_name, keystone_userinfo_t** userinfo);

    /**
     * Frees up all resources associated to the handle.
     * \note This is _not_ equivalent to calling ordinary free on the object. It is not possible to call free on the handle!
     * 
     * \param[in] handle a pointer to a valid userinfo handle obtained from either \sa{keystone_login} or \sa{keystone_get_userinfo_from_token}.
     * 
     * \return GSS_SUCCESS if everything went OK, something else otherwise.
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_free(keystone_userinfo_t* info);

    /**
     * Gets the userinfo from a token. This is typically used to authenticate a user. From the userinfo one can get the username and roles.
     * 
     * \param[in] handle a valid handle to keystone (obtained by \sa keystone_init).
     * 
     * \param[in] tenant_name a null terminated string containing the tenant_name
     *
     * \param[in] session_token a null terminated string containing the session_token
     *
     * \param[out] userinfo at end of execution, will contain a valid handle to a userinfo object. 
     *
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     *
     * \note All userinfo_objects must be freed with \sa keystone_userinfo_free
     *
     *  <example>
     * // assume keystone_handle is initialized
     * keystone_userinfo_t* userinfo_handle;
     * 
     * // assume session_token is obtained from somewhere...
     * keystone_error_t keystone_login_error = keystone_get_userinfo_from_token(keystone_handle, "tenant_name", session_token, &userinfo_handle);
     * 
     * if ( keystone_login_error != GSS_SUCCESS ) {
     *     // Something went wrong
     * }
     * 
     * // use it...
     * 
     * // remember to free it at the end:
     * keystone_error_t keystone_userinfo_free_error = keystone_userinfo_free(userinfo_handle);
     * 
     * if ( keystone_userinfo_free_error != GSS_SUCCESS ) {
     *     // Something went wrong...
     * }
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_get_userinfo_from_token(keystone_data_t* handle, const char* tenant_name, const char* session_token, keystone_userinfo_t** userinfo);

    /**
     * Gets the username associated to the userinfo. 
     * 
     * \note This expects to receive a large enough buffer, get the buffer size from \sa{keystone_userinfo_get_username_buffer_size}. 
     * 
     * \param[in] userinfo a pointer to a valid userinfo handle (obtained from eg. \sa{keystone_login} or \sa{keystone_get_userinfo_from_token}.
     * 
     * \param[out] buffer a buffer of at least the size returned from \sa{keystone_userinfo_get_username_buffer_size}. 
     *                    At the end of the execution, this will contain the username (with null termination). 
     * 
     * \param[in] buffer_length the total size of the buffer (this is expected to be at least the size of \sa{keystone_userinfo_get_username_buffer_size})
     *
     * \param[out] data_written at the end of the execution, will contain the number of bytes written to the buffer. 
     * 
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     *
     *
     *  <example>
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t required_buffer_size = 0;
     * keystone_error_t keystone_size_error = keystone_userinfo_get_username_buffer_size(userinfo_handle, &required_buffer_size);
     * if (keystone_size_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * 
     * // allocate a new buffer to hold the username:
     * char* username = (char*) malloc(required_buffer_size);
     * 
     * size_t data_written = 0;
     * 
     * keystone_error_t keystone_userinfo_error = keystone_userinfo_get_username(userinfo_handle, username, required_buffer_size, &data_written);
     * 
     *if (keystone_userinfo_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * // Let's print the result:
     * printf("username = %s\n", username);
     *
     * // We need to free the buffer ourselves, of course, using ordinary free (it was acquired with malloc):
     * free(username);
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_username(const keystone_userinfo_t* userinfo, char* buffer, size_t buffer_length, size_t* data_written);

    /**
     * Gets the buffer  size required to hold the username. 
     * 
     * \param[in] userinfo_handle a valid handle to a userinfo object, acquired from eg. \sa{keystone_login} or \sa{keystone_get_userinfo_from_token}
     * 
     * \param[out] size will at the end of execution contain the required size of the username buffer. 
     * 
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     *
     * <example>
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t required_buffer_size = 0;
     * keystone_error_t keystone_size_error = keystone_userinfo_get_username_buffer_size(userinfo_handle, &required_buffer_size);
     * if (keystone_size_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_username_buffer_size(const keystone_userinfo_t* userinfo_handle, size_t* size);

    /**
     * Gets the number of roles associated to the userinfo_handle. 
     * 
     * \param[in] userinfo_handle a valid handle to a userinfo object, acquired from eg. \sa{keystone_login} or \sa{keystone_get_userinfo_from_token}
     *
     * \param[out] role_count will at the end of execution contain the number of roles. 
     * 
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     * 
     * <example>
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t role_count = 0;
     * keystone_error_t keystone_count_error = keystone_userinfo_get_role_count(userinfo_handle, &role_count);
     * if (keystone_count_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role_count(const keystone_userinfo_t* userinfo_handle, size_t* role_count);


    /**
     * Gets the buffer size required to hold the the role at the given index. 
     * 
     * \param[in] userinfo_handle a valid handle to a userinfo object, acquired from eg. \sa{keystone_login} or \sa{keystone_get_userinfo_from_token}
     * 
     * \param[in] the index of the role
     * 
     * \param[out] size will at the end of execution contain the required size of the role buffer
     * 
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     *
     * <example>
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t required_buffer_size = 0;
     *
     * // Get the size for the first role (assume for the example that we have at least one role)
     * keystone_error_t keystone_size_error = keystone_userinfo_get_role_buffer_size(userinfo_handle, 0, &required_buffer_size);
     * if (keystone_size_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role_buffer_size(const keystone_userinfo_t* userinfo_handle, size_t index, size_t* buffer_size);

    /**
     * Gets the role associated to the userinfo at the given index. 
     * 
     * \note This expects to receive a large enough buffer, get the buffer size from \sa{keystone_userinfo_get_role_buffer_size}. 
     * 
     * \param[in] userinfo a pointer to a valid userinfo handle (obtained from eg. \sa{keystone_login} or \sa{keystone_get_userinfo_from_token}.
     * 
     * \param[in] index the index of the role (between 0 and the role_count obainted from \sa{keystone_get_role_count}
     *
     * \param[out] buffer a buffer of at least the size returned from \sa{keystone_userinfo_get_role_buffer_size}. 
     *                    At the end of the execution, this will contain the role (with null termination). 
     * 
     * \param[in] buffer_length the total size of the buffer (this is expected to be at least the size of \sa{keystone_userinfo_get_role_buffer_size})
     *
     * \param[out] data_written at the end of the execution, will contain the number of bytes written to the buffer. 
     * 
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     *
     *
     *  <example>
     * // assume keystone_handle and userinfo_handle are initialized.
     * // This example will loop through all roles and print them
     * 
     * // First get role_count;
     * size_t role_count = 0; 
     * gss_error_t keystone_role_count_error = keystone_userinfo_get_role_count(userinfo_handle, &role_count);
     * 
     * if ( keystone_role_count_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * 
     * for (int i = 0; i < role_count; i++) {
     *     size_t required_buffer_size = 0;
     *     keystone_error_t keystone_size_error = keystone_userinfo_get_role_buffer_size(userinfo_handle, i, &required_buffer_size);
     *     if (keystone_size_error != GSS_SUCCESS) {
     *         // Something went wrong
     *     }
     * 
     *     // allocate a new buffer to hold the role:
     *     char* role = (char*) malloc(required_buffer_size);
     * 
     *     size_t data_written = 0;
     *     
     *     keystone_error_t keystone_role_error = keystone_userinfo_get_role(uesrinfo_handle, i, role, required_buffer_size, &data_written);
     * 
     * 
     *     if (keystone_role_error != GSS_SUCCESS) {
     *         // Something went wrong
     *     }
     *     // Let's print the result:
     *     printf("role = %s\n", role);
     *
     *     // We need to free the buffer ourselves, of course, using ordinary free (it was acquired with malloc):
     *     free(role);
     * }
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role(const keystone_userinfo_t* userinfo, size_t index, char* buffer, size_t buffer_len, size_t* data_written);

    /**
     * Gets the token associated to the userinfo. 
     * 
     * \note This expects to receive a large enough buffer, get the buffer size from \sa{keystone_userinfo_get_token_buffer_size}. 
     * 
     * \param[in] userinfo a pointer to a valid userinfo handle (obtained from eg. \sa{keystone_login} or \sa{keystone_get_userinfo_from_token}.
     * 
     * \param[out] buffer a buffer of at least the size returned from \sa{keystone_userinfo_get_token_buffer_size}. 
     *                    At the end of the execution, this will contain the token (with null termination). 
     * 
     * \param[in] buffer_length the total size of the buffer (this is expected to be at least the size of \sa{keystone_userinfo_get_token_buffer_size})
     *
     * \param[out] data_written at the end of the execution, will contain the number of bytes written to the buffer. 
     * 
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     *
     *
     *  <example>
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t required_buffer_size = 0;
     * keystone_error_t keystone_size_error = keystone_userinfo_get_token_buffer_size(userinfo_handle, &required_buffer_size);
     * if (keystone_size_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * 
     * // allocate a new buffer to hold the token:
     * char* token = (char*) malloc(required_buffer_size);
     * 
     * size_t data_written = 0;
     * 
     * keystone_error_t keystone_userinfo_error = keystone_userinfo_get_token(userinfo_handle, token, required_buffer_size, &data_written);
     * 
     *if (keystone_userinfo_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * // Let's print the result:
     * printf("token = %s\n", token);
     *
     * // We need to free the buffer ourselves, of course, using ordinary free (it was acquired with malloc):
     * free(token);
     * </example>
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_token(const keystone_userinfo_t* userinfo, char* buffer, size_t buffer_length, size_t* data_written);


    /**
     * Gets the buffer  size required to hold the token. 
     * 
     * \param[in] userinfo_handle a valid handle to a userinfo object, acquired from eg. \sa{keystone_login} or \sa{keystone_get_userinfo_from_token}
     * 
     * \param[out] size will at the end of execution contain the required size of the token buffer. 
     * 
     * \return GSS_SUCCESS if all went OK, something else otherwise. 
     *
     * <example>
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t required_buffer_size = 0;
     * keystone_error_t keystone_size_error = keystone_userinfo_get_token_buffer_size(userinfo_handle, &required_buffer_size);
     * if (keystone_size_error != GSS_SUCCESS) {
     *     // Something went wrong
     * }
     * </example>
     */
    
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_token_buffer_size(const keystone_userinfo_t* userinfo_handle, size_t* size);

#ifdef __cplusplus
}
#endif
