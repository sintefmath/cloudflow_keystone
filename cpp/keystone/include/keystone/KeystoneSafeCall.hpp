#pragma once
#include <stdexcept>
#include <iostream>

/**
 * \file
 */
/**
 * \example keystone_safe_call_example
 * \code{.c}
 * keystone_data_t* data;
 * // This will throw an exception if anything went wrong: 
 * KEYSTONE_SAFE_CALL(keystone_init("http://server/", data);
 * \endcode
 */

/**
 *!\public 
 * \brief short utility macro that calls an exception when a method returns a non \ref KEYSTONE_SUCCESS value
 * 
 */
#define KEYSTONE_SAFE_CALL(x) { \
    if ( x != KEYSTONE_SUCCESS) { \
    std::cout << "--- KEYSTONE_SAFE_CALL throwing error " << x << "--------" << std::endl; \
    throw std::runtime_error("Keystone generated an error"); \
    } \
}
