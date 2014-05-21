#pragma once
#include <stdexcept>

#define KEYSTONE_SAFE_CALL(x) { \
    if ( x != KEYSTONE_SUCCESS) { \
    throw std::runtime_error("Keystone generated an error"); \
    } \
}