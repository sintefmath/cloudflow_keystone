PROJECT( keystone_cpp )
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                           ${CMAKE_SOURCE_DIR}/cmake/Modules)
						   
# We are going to need pugixml for everything:
FIND_PACKAGE(PugiXML REQUIRED)
FIND_PACKAGE(CURL REQUIRED)
INCLUDE_DIRECTORIES("keystone/include" ${CMAKE_BINARY_DIR}/keystone ${PUGIXML_INCLUDE_DIR} ${CURL_INCLUDE_DIR})

ADD_SUBDIRECTORY(keystone)
ADD_SUBDIRECTORY(keystone_login)
ADD_SUBDIRECTORY(keystone_check)
