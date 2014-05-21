FILE(GLOB ROLES_SRC "*.cpp" "*.hpp")

ADD_EXECUTABLE(keystone_roles ${ROLES_SRC})

TARGET_LINK_LIBRARIES(keystone_roles ${CURL_LIBRARY} keystone)