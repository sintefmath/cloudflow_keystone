FILE(GLOB CHECK_SRC "*.cpp" "*.hpp")

ADD_EXECUTABLE(keystone_check ${CHECK_SRC})

TARGET_LINK_LIBRARIES(keystone_check ${CURL_LIBRARY} keystone)
