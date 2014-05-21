INCLUDE (GenerateExportHeader)

FILE(GLOB_RECURSE LIBRARY_SRC "*.cpp" "*.hpp" "*.h")

FILE(GLOB KEYSTONE_IMPL_SRC "src/keystone/impl/*.cpp")
FILE(GLOB KEYSTONE_IMPL_HEADERS "include/keystone/impl/*.hpp")
FILE(GLOB KEYSTONE_SRC "src/keystone/*.cpp")
FILE(GLOB KEYSTONE_HEADERS "include/impl/*.hpp" "include/impl/*.c")

SOURCE_GROUP("Source Files\\keystone\\impl" FILES ${KEYSTONE_IMPL_SRC})
SOURCE_GROUP("Header Files\\keystone\\impl" FILES ${KEYSTONE_IMPL_HEADERS})
SOURCE_GROUP("Header Files\\keystone" FILES ${KEYSTONE_HEADERS})
SOURCE_GROUP("Source Files\\keystone" FILES ${KEYSTONE_SRC})

ADD_LIBRARY(keystone SHARED ${LIBRARY_SRC})

add_compiler_export_flags()

GENERATE_EXPORT_HEADER( keystone
             BASE_NAME keystone
             EXPORT_MACRO_NAME KEYSTONE_EXPORT
             EXPORT_FILE_NAME keystone/keystone_export.h
             STATIC_DEFINE KEYSTONE_BUILT_AS_STATIC
)



TARGET_LINK_LIBRARIES(keystone debug ${PUGIXML_LIBRARY_DEBUG} ${CURL_LIBRARY})
TARGET_LINK_LIBRARIES(keystone optimized ${PUGIXML_LIBRARY} ${CURL_LIBRARY})
