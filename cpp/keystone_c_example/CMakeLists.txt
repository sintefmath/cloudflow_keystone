FILE(GLOB C_EXAMPLE_SRC "*.c" "*.h")

ADD_EXECUTABLE(keystone_c_example ${C_EXAMPLE_SRC})

TARGET_LINK_LIBRARIES(keystone_c_example keystone)