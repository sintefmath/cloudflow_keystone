FILE(GLOB LOGIN_SRC "*.cpp" "*.hpp")

ADD_EXECUTABLE(keystone_login_https ${LOGIN_SRC})

TARGET_LINK_LIBRARIES(keystone_login_https ${CURL_LIBRARY} keystone )