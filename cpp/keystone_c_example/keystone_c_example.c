#include "keystone/keystone.h"
#include "stdio.h"

int main(int argc, char** argv) {
    // We need to write in old C-style (MSVC does not support C99)
    char * url;
    char* username;
    char* password;
    char* tenant_name;
    keystone_data_t* keystone_handle;
    keystone_userinfo_t* userinfo_handle;
    size_t required_buffer_size;
    size_t data_written;
    size_t role_count;
    char* buffer;
    int i;

    if(argc != 5) {
	printf("Usage: \n");
        printf("\tkeystone_c_example <URL> <username> <password> <tenantname>\n");
        return 1;
    }
    
    url = argv[1];
    username = argv[2];
    password = argv[3];
    tenant_name = argv[4];

    /* First we initialize keystone: */
    keystone_data_t* keystone;
    if (keystone_init(url, &keystone_handle) != KEYSTONE_SUCCESS) {
	printf("Error when initializing keystone\n");
	return 1;
    }
    
    /* Then we can log the user in:  */
    if (keystone_login(keystone_handle, username, password, tenant_name, &userinfo_handle) != KEYSTONE_SUCCESS) {
	printf("Could not login\n");
	keystone_free(keystone_handle);
	return 1;
    }

 

    /* Next we can also print out the username (not really needed since we already have it... 
       but if one is given only a session, one would do this to get the username) */
    /* First we must allocate enough data */
    if (keystone_userinfo_get_username_buffer_size(userinfo_handle, &required_buffer_size) != KEYSTONE_SUCCESS) {
	printf("Could not get buffersize\n");
	keystone_free(keystone_handle);
	keystone_userinfo_free(userinfo_handle);
	return 1;
    }

    /** Then we allocate: */
    buffer = (char*)malloc(required_buffer_size);
    
    /** Then we read: */
    if (keystone_userinfo_get_username(userinfo_handle, buffer, required_buffer_size, &data_written) != KEYSTONE_SUCCESS) {
	printf("Could not get username\n");
	keystone_free(keystone_handle);
	keystone_userinfo_free(userinfo_handle);
	free(buffer);
	return 1;
    }

    /* and finally we can print: */
    printf("username from login: \"%s\"\n\n", buffer);

    /* Free buffer: */
    free(buffer);

    /* We can also print out the roles: */
    if (keystone_userinfo_get_role_count(userinfo_handle, &role_count) != KEYSTONE_SUCCESS) {
	printf("Could not get role count\n");
	keystone_free(keystone_handle);
	keystone_userinfo_free(userinfo_handle);
	return 1;
    }

    for(i = 0; i < role_count; i++) {
	/* First we must allocate enough data */
	if (keystone_userinfo_get_role_buffer_size(userinfo_handle, i, &required_buffer_size) != KEYSTONE_SUCCESS) {
	    printf("Could not get buffersize\n");
	    keystone_free(keystone_handle);
	    keystone_userinfo_free(userinfo_handle);
	    return 1;
	}

	/*  Then we allocate: */
	buffer = (char*)malloc(required_buffer_size);
    
	/* Then we read: */
	if (keystone_userinfo_get_role(userinfo_handle, i, buffer, required_buffer_size, &data_written) != KEYSTONE_SUCCESS) {
	    printf("Could not get username\n");
	    keystone_free(keystone_handle);
	    keystone_userinfo_free(userinfo_handle);
	    free(buffer);
	    return 1;
	}

	/* and finally we can print: */
	printf("role[%d] = %s\n", i, buffer);
	
	/* Free up buffer for next round.  */
	free(buffer);
    }
    printf("\n");

    /* Let's print out the session token */
    /* First we must allocate enough data */
    if (keystone_userinfo_get_token_buffer_size(userinfo_handle, &required_buffer_size) != KEYSTONE_SUCCESS) {
	printf("Could not get buffersize\n");
	keystone_free(keystone_handle);
	keystone_userinfo_free(userinfo_handle);
	return 1;
    }

    /* Then we allocate: */
    buffer = (char*)malloc(required_buffer_size);
    
    /* Then we read: */
    if (keystone_userinfo_get_token(userinfo_handle, buffer, required_buffer_size, &data_written) != KEYSTONE_SUCCESS) {
	printf("Could not get token\n");	
	keystone_free(keystone_handle);
	keystone_userinfo_free(userinfo_handle);
	free(buffer);
	return 1;
    }

    /* and finally we can print: */
    printf("token from login: \"%s\"\n", buffer);



    /* Now we can delete the userinfo-object: */
    if (keystone_userinfo_free(userinfo_handle) != KEYSTONE_SUCCESS) {
	printf("Could not free userinfo");
	keystone_free(keystone_handle);
	free(buffer);
	return 1;
    }

    printf("\n\nTrying to get userinfo directly from token\n\n");
    /* And we can again get the userinfo from a session-token (the one we just got):  */
    if (keystone_get_userinfo_from_token(keystone_handle, tenant_name, buffer, &userinfo_handle) != KEYSTONE_SUCCESS) {
	keystone_free(keystone_handle);
	free(buffer);
	printf("Could not get user info from token");
	return 1;
    }

    /* And now we may free the buffer:  */
    free(buffer);

    /* And we can get all the information again, starting with username: 
	First we must allocate enough data */
    if (keystone_userinfo_get_username_buffer_size(userinfo_handle, &required_buffer_size) != KEYSTONE_SUCCESS) {
	printf("Could not get buffersize\n");
	
	keystone_free(keystone_handle);
	keystone_userinfo_free(userinfo_handle);
	return 1;
    }

    /* Then we allocate: */
    buffer = (char*)malloc(required_buffer_size);
    
    /* Then we read: */
    if (keystone_userinfo_get_username(userinfo_handle, buffer, required_buffer_size, &data_written) != KEYSTONE_SUCCESS) {
	printf("Could not get username\n");
	keystone_free(keystone_handle);
	keystone_userinfo_free(userinfo_handle);
	free(buffer);
	return 1;
    }

    /* and finally we can print: */
    printf("username from token: \"%s\"\n\n", buffer);

    /* Free buffer: */
    free(buffer);

    /* We can also print out the roles: */
    if (keystone_userinfo_get_role_count(userinfo_handle, &role_count) != KEYSTONE_SUCCESS) {
	printf("Could not get role count\n");
	keystone_free(keystone_handle);
	keystone_userinfo_free(userinfo_handle);
	return 1;
    }

    for(i = 0; i < role_count; i++) {
	/* First we must allocate enough data */
	if (keystone_userinfo_get_role_buffer_size(userinfo_handle, i, &required_buffer_size) != KEYSTONE_SUCCESS) {
	    printf("Could not get buffersize\n");
	    keystone_free(keystone_handle);
	    keystone_userinfo_free(userinfo_handle);
	    return 1;
	}

	/* Then we allocate: */
	buffer = (char*)malloc(required_buffer_size);
    
	/* Then we read:*/
	if (keystone_userinfo_get_role(userinfo_handle, i, buffer, required_buffer_size, &data_written) != KEYSTONE_SUCCESS) {
	    printf("Could not get username\n");
	    keystone_free(keystone_handle);
	    keystone_userinfo_free(userinfo_handle);
	    free(buffer);
	    return 1;
	}

	/* and finally we can print: */
	printf("from token role[%d] = %s\n", i, buffer);
	
	/* Free up buffer for next round. */
	free(buffer);
    }

    // Finally we free up the objects:
    keystone_userinfo_free(userinfo_handle);
    keystone_free(keystone_handle);
    return 0;
}
