Short
============
This is a short collection of utility function for the OpenStack Keystone service.

It contains:
   * keystone: a standalone library for using keystone
   * keystone_login: an executable program for getting a session token
   * keystone_check: an executable program for checking a session token.
   * keystone_roles: an executable program for listing roles of a user.
   * keystone_https: an executable program for getting a session token while specifying location of CA certificat file.
   * keystone_c_example: an example program using the DLL interface in C. 
   
Building
==============
##Requirements:
  * cmake (http://www.cmake.org/)
  * C++03 compiler (tested with GCC and Visual Studio 2010)
  * libcurl (http://curl.haxx.se/libcurl/, tested version 7.36)
  * doxygen *optional* (http://www.stack.nl/~dimitri/doxygen/index.html)
  
For Windows we recommend using the precompiled version of libcurl found at http://www.confusedbycode.com/curl/

## Issuing cmake:
To use cmake you may simply issue:

    cmake <(relative) path to keystone_cpp folder>

This should generate the nessecary build files (either a Visual Studio project in Windows or makefile in Linux).


keystone_login
==============
The keystone_login program expects a url (on the form "http://server.com/keystone/"). Note 
that the "v2.0"-part is left out, a valid username, password and tenantid. The correct form is:

    keystone_login <URL> <USERNAME> <PASSWORD> <TENANTID>

It returns 0 and prints the session token to stdout if everything went OK,
and returns a non-zero value if there was an error (and writes a short error message to stderr)

keystone_check
==============
The keystone_check program expects a url (on the form "http://server.com/keystone/"). Note 
that the "v2.0"-part is left out, a tenant id and a session token. The correct from is:

    keystone_check <URL> <TENANTID> <SESSIONTOKEN>

It returns 0 and prints the username associated to the token to stdout if everything went OK,
and returns a non-zero value if there was an error (and writes a short error message to stderr).


keystone_https
==============
The keystone_https program is very similar to keystone_login and prints the session token to stdout if
everything went OK, and returns 0. However, in order to use the https protocol, we sometimes need to provide the information
on where to find the certificate bundle fra the Certificate Authorities (CA).

Such a file can be downloaded from e.g. http://curl.haxx.se/ca/cacert.pem and should be stored locally.
The path to this file (including its file name) can then be used as a optional parameter to this program.

    keystone_https <URL> <USERNAME> <PASSWORD> <TENANTID> [<CERTFILENAME>]

The program will return a non-zero value and print a short error message to stderr in case something goes wrong.


##Specify certificate file by environment variable

If you need to access a keystone server under https protocol and you can not use the function for setting the path to the certificate bundle as shown in the keystone_https example, you can instead use an environmental variable to get the same behaviour.

For instance, use keystone_login example as follows (using Linux):

    export KEYSTONE_SET_CA_CERTIFICATE_FILENAME=<path_to_certification_file>
    keystone_login https://<your_domain>/keystone/ <USERNAME> <PASSWORD> <TENANTID>

