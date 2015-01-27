PROJECT( keystone_cpp )
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                           ${CMAKE_SOURCE_DIR}/cmake/Modules)
						   
# We are going to need pugixml for everything, but its source code is 
# now in this project.
FIND_PACKAGE(CURL REQUIRED)
INCLUDE_DIRECTORIES("keystone/include" "keystone/pugi4lunch" ${CMAKE_BINARY_DIR}/keystone ${CURL_INCLUDE_DIR})

ADD_SUBDIRECTORY(keystone)
ADD_SUBDIRECTORY(keystone_login)
ADD_SUBDIRECTORY(keystone_check)
ADD_SUBDIRECTORY(keystone_roles)
ADD_SUBDIRECTORY(keystone_c_example)
SET(DOXYGEN_SKIP_DOT ON)
FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
	ADD_SUBDIRECTORY(doxygen)
ENDIF()