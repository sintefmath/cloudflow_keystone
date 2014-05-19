Short
============
This is a short collection of utility function for the OpenStack Keystone service.

It contains:
   keystone: a standalone library for using keystone
   keystone_login: an executable program for getting a session token
   keystone_check: an executable program for checking a session token.
   
Building
==============
##Requirements:
  * cmake (http://www.cmake.org/)
  * C++03 compiler (tested with GCC and Visual Studio 2010)
  * libcurl (http://curl.haxx.se/libcurl/, tested version 7.36)
  * pugixml (http://pugixml.org/)

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


