FILE(GLOB LOGIN_SRC "*.cpp" "*.hpp")

ADD_EXECUTABLE(keystone_https ${LOGIN_SRC})

TARGET_LINK_LIBRARIES(keystone_https ${CURL_LIBRARY} keystone)
