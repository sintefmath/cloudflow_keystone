FILE(GLOB LOGIN_SRC "*.cpp" "*.hpp")

ADD_EXECUTABLE(keystone_login ${LOGIN_SRC})

TARGET_LINK_LIBRARIES(keystone_login ${CURL_LIBRARY} keystone)
