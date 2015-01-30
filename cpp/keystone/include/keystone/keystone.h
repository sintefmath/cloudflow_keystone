#pragma once

// This is to get the DLL-definitions.
// It must not be removed. It should have  been generated automatically by cmake
#include "keystone/keystone_export.h"

/** 
 * \file
 * 
 * \brief this contains the main C-interface for accessing the KeyStone service.
 * 
 * 
 * \addtogroup keystone
 * \brief The standard C-API. This is the API exposed in the DLL interface. 
 * \{
 */
// For size_t: 
#include "stdlib.h"

/**
 * This will hold data related to the keystone service. Access with the relevant methods.
 * \note We do not expose the structure of this struct, all data must be obtained from the getter methods.
 */
struct keystone_data_struct;


/**
 * This will hold data related to the keystone service. Access with the relevant methods.
 * \note We do not expose the structure of this struct, all data must be obtained from the getter methods.
 */
typedef struct keystone_data_struct keystone_data_t;


/**
 * This will hold data related to the keystone service. Access with the relevant methods.
 * \note We do not expose the structure of this struct, all data must be obtained from the getter methods.
 */
struct keystone_userinfo_struct;

/**
 * This will hold data related to the keystone service. Access with the relevant methods.
 * \note We do not expose the structure of this struct, all data must be obtained from the getter methods.
 */
typedef struct keystone_userinfo_struct keystone_userinfo_t;

/**
 *! \public
 * The error values returned by keystone functions
 */
typedef enum {
    /**
     * Returned if everything went OK
     */
    KEYSTONE_SUCCESS = 0,

    /**
     * Returned when cause of error is unknown
     */
    KEYSTONE_UNKNOWN_ERROR  
} keystone_error_t;
#ifdef __cplusplus
extern "C" {
#endif
    /**
     * \example keystone_c_example.c
     */
        
    /**
     * \ingroup keystone
     * \example keystone_init_example
     * \code{.c}
     * keystone_data_t* keystone_handle;
     * keystone_error_t keystone_init_error = keystone_init("http://server.com/keystone/", &keystone_handle);
     * 
     * if ( keystone_init_error != KEYSTONE_SUCCESS ) {
     *     // Something went wrong
     * }
     * 
     * // use it...
     * 
     * // remember to free it at the end:
     * keystone_error_t keystone_free_error = keystone_free(keystone_handle);
     * 
     * if ( keystone_free_error != KEYSTONE_SUCCESS ) {
     *     // Something went wrong...
     * }
     * \endcode
     */

    /**
     * Creates a new instance of the keystone object. All further calls expects an instance created from this function.
     * 
     * \param[in] url a (null terminated) string containing the URL for the keystone service. This is typically on the form
     *                http://server/keystone/ (that it is, without the "v2.0/"-part afterwards, it will be added).
     *                Also note that we require the trailing dash '/' at the end (http://server/keystone/ is valid, http://server/keystone is _not_)
     * 
     * \param[out] handle a pointer to a pointer to a data. At the end of a successful run, this will contain a valid pointer to a keystone_handle
     * 
     * \return \ref KEYSTONE_SUCCESS if everything went OK, something else if there was an error.
     *
     * \note All data objects initialized with \ref keystone_init _must_ be freed with \ref keystone_free (ordinary \c free will not work!)
     *
     */
    KEYSTONE_EXPORT keystone_error_t keystone_init(const char* url, keystone_data_t** handle);


    /**
     * \ingroup keystone
     * Frees up all resources associated to the handle.
     * \note This is _not_ equivalent to calling ordinary \c free on the object. It is not possible to call \c free on the handle!
     *
     * \note This does _not_ free up other data created from the keystone-C-API. In other words, it will _not_ free up \ref keystone_userinfo_t resources, 
     *        those must be freed with \ref keystone_userinfo_free
     * 
     * \sa keystone_init 
     * \sa keystone_userinfo_free
     *
     * \param[in] handle a pointer to a valid keystone_handle obtained from [keystone_init](@ref keystone_init)
     * \return \ref KEYSTONE_SUCCESS if everything went OK, something else otherwise.
     */
    KEYSTONE_EXPORT keystone_error_t keystone_free(keystone_data_t* handle);

    /**
     *
     * \example keystone_login_example
     * \code{.c}
     * // assume keystone_handle is initialized
     * keystone_userinfo_t* userinfo_handle;
     * keystone_error_t keystone_login_error = keystone_login(keystone_handle, "username", "password", "tenant_name", &userinfo_handle);
     * 
     * if ( keystone_login_error != KEYSTONE_SUCCESS ) {
     *     // Something went wrong
     * }
     * 
     * // use it...
     * 
     * // remember to free it at the end:
     * keystone_error_t keystone_userinfo_free_error = keystone_userinfo_free(userinfo_handle);
     * 
     * if ( keystone_userinfo_free_error != KEYSTONE_SUCCESS ) {
     *     // Something went wrong...
     * }
     * \endcode
     * 
     */

    /**
     * \ingroup keystone
     *
     * Logs the user in and gives back a session token. 
     * 
     * \sa keystone_init
     * \param[in] handle a valid handle to keystone as obtained by [keystone_init](@ref keystone_init)
     * 
     * \param[in] username a null terminated string containing the username
     *
     * \param[in] password a null terminated string containing the password
     * 
     * \param[in] tenant_name a null terminated string containing the tenant_name
     * 
     * \param[out] userinfo at end of execution, will contain a valid handle to a userinfo object. 
     * 
     * \return [KEYSTONE_SUCCESS](@ref keystone_error_t::KEYSTONE_SUCCESS) if all went OK, something else otherwise. 
     * 
     * \note All userinfo_objects must be freed with \ref keystone_userinfo_free
     */
    KEYSTONE_EXPORT keystone_error_t keystone_login(keystone_data_t* handle, const char* username, const char* password, const char* tenant_name, keystone_userinfo_t** userinfo);



    /**
     * 
     * \ingroup keystone
     * 
     * Frees up all resources associated to the handle.
     * \note This is _not_ equivalent to calling ordinary \c free on the object. It is not possible to call \c free on the handle!
     * 
     * \param[in] handle a pointer to a valid userinfo handle obtained from either \ref keystone_login or \ref keystone_get_userinfo_from_token.
     * 
     * \return \ref KEYSTONE_SUCCESS if everything went OK, something else otherwise.
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_free(keystone_userinfo_t* handle);


    /**
    * \example get_userinfo_from_token_example
    * \code{.c}
    * // assume keystone_handle is initialized
    * keystone_userinfo_t* userinfo_handle;
    * 
    * // assume session_token is obtained from somewhere...
    * keystone_error_t keystone_login_error = keystone_get_userinfo_from_token(keystone_handle, "tenant_name", session_token, &userinfo_handle);
    * 
    * if ( keystone_login_error != KEYSTONE_SUCCESS ) {
    *     // Something went wrong
    * }
    * 
    * // use it...
    * 
    * // remember to free it at the end:
    * keystone_error_t keystone_userinfo_free_error = keystone_userinfo_free(userinfo_handle);
    * 
    * if ( keystone_userinfo_free_error != KEYSTONE_SUCCESS ) {
    *     // Something went wrong...
    * }
    * \endcode
    */

    /**
     * \ingroup keystone
     * Gets the userinfo from a token. This is typically used to authenticate a user. 
     * From the userinfo one can get the [username](\ref keystone_userinfo_get_username) and 
     * [roles](\ref keystone_userinfo_get_role).
     * 
     * \sa keystone_userinfo_free
     * \sa keystone_userinfo_get_username
     * \sa keystone_userinfo_get_token
     * \sa keystone_userinfo_get_role_count
     * \sa keystone_userinfo_get_role
     * 
     * \param[in] handle a valid handle to keystone (obtained by \ref keystone_init).
     * 
     * \param[in] tenant_name a null terminated string containing the tenant_name
     *
     * \param[in] session_token a null terminated string containing the session_token
     *
     * \param[out] userinfo at end of execution, will contain a valid handle to a userinfo object. 
     *
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise. 
     *
     * \note All userinfo_objects must be freed with \ref keystone_userinfo_free
     */
    KEYSTONE_EXPORT keystone_error_t keystone_get_userinfo_from_token(keystone_data_t* handle, const char* tenant_name, const char* session_token, keystone_userinfo_t** userinfo);


    /**
     * \ingroup keystone
     * Set the path to the file containing the certificates from the Certificate Authorities (CA). This might be required for accessing
     * keystone servers behind https protocols.
     *
     * \param[in] handle a valid handle to keystone (obtained by \ref keystone_init).
     *
     * \param[in] cert_file_name a null terminated string containing the path and file name of the CA bundle.
     *
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise.
     */
    KEYSTONE_EXPORT keystone_error_t keystone_set_ca_certificate_filename(keystone_data_t* handle, const char* cert_file_name);


    /**
    * \example keystone_get_username_example 
    * \code{.c}
    * // assume keystone_handle and userinfo_handle are initialized.
    * 
    * size_t required_buffer_size = 0;
    * keystone_error_t keystone_size_error = keystone_userinfo_get_username_buffer_size(userinfo_handle, &required_buffer_size);
    * if (keystone_size_error != KEYSTONE_SUCCESS) {
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
    *if (keystone_userinfo_error != KEYSTONE_SUCCESS) {
    *     // Something went wrong
    * }
    * // Let's print the result:
    * printf("username = %s\n", username);
    *
    * // We need to free the buffer ourselves, of course, using ordinary free (it was acquired with malloc):
    * free(username);
    * \endcode
     */

    /**
     * \ingroup keystone
     * Gets the username associated to the userinfo. 
     * 
     * \note This expects to receive a large enough buffer, get the buffer size from \ref keystone_userinfo_get_username_buffer_size
     * 
     * \sa keystone_userinfo_get_username_buffer_size
     *
     * \param[in] userinfo a pointer to a valid userinfo handle (obtained from eg. \ref keystone_login or \ref keystone_get_userinfo_from_token
     * 
     * \param[out] buffer a buffer of at least the size returned from \ref keystone_userinfo_get_username_buffer_size.
     *                    At the end of the execution, this will contain the username (with null termination). 
     * 
     * \param[in] buffer_length the total size of the buffer (this is expected to be at least the size of \ref keystone_userinfo_get_username_buffer_size)
     *
     * \param[out] data_written at the end of the execution, will contain the number of bytes written to the buffer. 
     * 
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise. 
     *
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_username(const keystone_userinfo_t* userinfo, char* buffer, size_t buffer_length, size_t* data_written);


    /**
    * \example keystone_userinfo_get_buffer_size_example 
    * \code{.c}
    * // assume keystone_handle and userinfo_handle are initialized.
    * 
    * size_t required_buffer_size = 0;
    * keystone_error_t keystone_size_error = keystone_userinfo_get_username_buffer_size(userinfo_handle, &required_buffer_size);
    * if (keystone_size_error != KEYSTONE_SUCCESS) {
    *     // Something went wrong
    * }
    * \endcode
    */

    /**
     * \ingroup keystone
     * 
     * Gets the buffer size required to hold the username. 
     * 
     * \sa keystone_userinfo_get_username
     * 
     * \param[in] userinfo_handle a valid handle to a userinfo object, acquired from eg. \ref keystone_login or \ref keystone_get_userinfo_from_token
     * 
     * \param[out] size will at the end of execution contain the required size of the username buffer. 
     * 
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise. 
     * 
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_username_buffer_size(const keystone_userinfo_t* userinfo_handle, size_t* size);


    /**
     * \example keystone_userinfo_get_role_count_example
     * \code{.c}
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t role_count = 0;
     * keystone_error_t keystone_count_error = keystone_userinfo_get_role_count(userinfo_handle, &role_count);
     * if (keystone_count_error != KEYSTONE_SUCCESS) {
     *     // Something went wrong
     * }
     * \endcode
     */

    /**
     * \ingroup keystone
     * 
     * Gets the number of roles associated to the userinfo_handle. 
     * 
     * \sa keystone_userinfo_get_role
     * \sa keystone_userinfo_get_role_buffer_size
     * 
     * \param[in] userinfo_handle a valid handle to a userinfo object, acquired from eg. \ref keystone_login or \ref keystone_get_userinfo_from_token
     *
     * \param[out] role_count will at the end of execution contain the number of roles. 
     * 
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise. 
     * 
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role_count(const keystone_userinfo_t* userinfo_handle, size_t* role_count);



    /**
     * 
     * \example keystone_userinfo_get_role_buffer_size
     * \code{.c}
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t required_buffer_size = 0;
     *
     * // Get the size for the first role (assume for the example that we have at least one role)
     *
     * keystone_error_t keystone_size_error = keystone_userinfo_get_role_buffer_size(userinfo_handle, 0, &required_buffer_size);
     *
     * if (keystone_size_error != KEYSTONE_SUCCESS) {
     *     // Something went wrong
     * }
     * \endcode
     */

    /**
     * \ingroup keystone
     *
     * Gets the buffer size required to hold the the role at the given index. 
     * 
     * \sa keystone_userinfo_get_role
     * \sa keystone_userinfo_get_role_count
     *
     * \param[in] userinfo_handle a valid handle to a userinfo object, acquired from eg. \ref keystone_login or \ref keystone_get_userinfo_from_token
     * 
     * \param[in] index the index of the role (between 0 and the role_count obtained from \ref keystone_userinfo_get_role_count, not inclusive)
     * 
     * \param[out] buffer_size will at the end of execution contain the required size of the role buffer
     * 
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise. 
     *
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role_buffer_size(const keystone_userinfo_t* userinfo_handle, size_t index, size_t* buffer_size);

    /**
    *  \example keystone_list_roles 
    * \code{.c}
    * // assume keystone_handle and userinfo_handle are initialized.
    * // This example will loop through all roles and print them
    * 
    * // First get role_count;
    * size_t role_count = 0; 
    * gss_error_t keystone_role_count_error = keystone_userinfo_get_role_count(userinfo_handle, &role_count);
    * 
    * if ( keystone_role_count_error != KEYSTONE_SUCCESS) {
    *     // Something went wrong
    * }
    * 
    * for (int i = 0; i < role_count; i++) {
    *     size_t required_buffer_size = 0;
    *     keystone_error_t keystone_size_error = keystone_userinfo_get_role_buffer_size(userinfo_handle, i, &required_buffer_size);
    *     if (keystone_size_error != KEYSTONE_SUCCESS) {
    *         // Something went wrong
    *     }
    * 
    *     // allocate a new buffer to hold the role:
    *     char* role = (char*) malloc(required_buffer_size);
    * 
    *     size_t data_written = 0;
    *     
    *     keystone_error_t keystone_role_error = keystone_userinfo_get_role(userinfo_handle, i, role, required_buffer_size, &data_written);
    * 
    * 
    *     if (keystone_role_error != KEYSTONE_SUCCESS) {
    *         // Something went wrong
    *     }
    *     // Let's print the result:
    *     printf("role = %s\n", role);
    *
    *     // We need to free the buffer ourselves, of course, using ordinary free (it was acquired with malloc):
    *     free(role);
    * }
    * \endcode
    **/

    /**
     * \ingroup keystone
     * 
     * Gets the role associated to the userinfo at the given index. 
     * 
     * \note This expects to receive a large enough buffer, get the buffer size from \ref keystone_userinfo_get_role_buffer_size.
     * 
     * \param[in] userinfo a pointer to a valid userinfo handle (obtained from eg. \ref keystone_login or \ref keystone_get_userinfo_from_token.
     * 
     * \param[in] index the index of the role (between 0 and the role_count obtained from \ref keystone_userinfo_get_role_count, not inclusive)
     *
     * \param[out] buffer a buffer of at least the size returned from \ref keystone_userinfo_get_role_buffer_size. 
     *                    At the end of the execution, this will contain the role (with null termination). 
     * 
     * \param[in] buffer_length the total size of the buffer (this is expected to be at least the size of \ref keystone_userinfo_get_role_buffer_size
     *
     * \param[out] data_written at the end of the execution, will contain the number of bytes written to the buffer. 
     * 
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise. 
     *
     *
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_role(const keystone_userinfo_t* userinfo, size_t index, char* buffer, size_t buffer_length, size_t* data_written);


    /**
    * \example keystone_userinfo_get_token
    * \code{.c}
    * // assume keystone_handle and userinfo_handle are initialized.
    * 
    * size_t required_buffer_size = 0;
    * keystone_error_t keystone_size_error = keystone_userinfo_get_token_buffer_size(userinfo_handle, &required_buffer_size);
    * if (keystone_size_error != KEYSTONE_SUCCESS) {
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
    *if (keystone_userinfo_error != KEYSTONE_SUCCESS) {
    *     // Something went wrong
    * }
    * // Let's print the result:
    * printf("token = %s\n", token);
    *
    * // We need to free the buffer ourselves, of course, using ordinary free (it was acquired with malloc):
    * free(token);
    * \endcode
    */
    
    /**
     * \ingroup keystone
     * 
     * Gets the token associated to the userinfo. 
     * 
     * \sa keystone_userinfo_get_token_buffer_size
     * 
     * \note This expects to receive a large enough buffer, get the buffer size from \ref keystone_userinfo_get_token_buffer_size. 
     * 
     * \param[in] userinfo a pointer to a valid userinfo handle (obtained from eg. \ref keystone_login or \ref keystone_get_userinfo_from_token.
     * 
     * \param[out] buffer a buffer of at least the size returned from \ref keystone_userinfo_get_token_buffer_size. 
     *                    At the end of the execution, this will contain the token (with null termination). 
     * 
     * \param[in] buffer_length the total size of the buffer (this is expected to be at least the size returned from \ref keystone_userinfo_get_token_buffer_size)
     *
     * \param[out] data_written at the end of the execution, will contain the number of bytes written to the buffer. 
     * 
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise. 
     */
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_token(const keystone_userinfo_t* userinfo, char* buffer, size_t buffer_length, size_t* data_written);


    /**
     * \example keystone_userinfo_get_token_buffer_size
     * \code{.c}
     * // assume keystone_handle and userinfo_handle are initialized.
     * 
     * size_t required_buffer_size = 0;
     * keystone_error_t keystone_size_error = keystone_userinfo_get_token_buffer_size(userinfo_handle, &required_buffer_size);
     * if (keystone_size_error != KEYSTONE_SUCCESS) {
     *     // Something went wrong
     * }
     *\endcode
     */

    /**
     * \ingroup keystone
     * 
     * Gets the buffer  size required to hold the token. 
     * 
     * \sa keystone_userinfo_get_token
     * 
     * \param[in] userinfo_handle a valid handle to a userinfo object, acquired from eg. \ref keystone_login or \ref keystone_get_userinfo_from_token
     * 
     * \param[out] size will at the end of execution contain the required size of the token buffer. 
     * 
     * \return \ref KEYSTONE_SUCCESS if all went OK, something else otherwise. 
     *
     */  
    KEYSTONE_EXPORT keystone_error_t keystone_userinfo_get_token_buffer_size(const keystone_userinfo_t* userinfo_handle, size_t* size);

#ifdef __cplusplus
}
#endif
/**
 * \}
 */
