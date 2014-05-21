#include "keystone/Keystone.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {

    if(argc != 4) {
        std::cout << "Usage: " <<std::endl;
        std::cout << "\tkeystone_roles <URL> <tenantname> <sessiontoken>" << std::endl;
        return 1;
    }
    
    const std::string url(argv[1]);
    const std::string tenantName(argv[2]);
    const std::string sessionToken(argv[3]);
    

    keystone::Keystone keystone(url);
    
    try {
        keystone::KeystoneUserInfo info;
        keystone.getUserInfoFromToken(tenantName, sessionToken, info);
	for(size_t i = 0; i < info.getRoleCount(); i++) {
	    std::cout << info.getRole(i) << std::endl;
	}
        return 0;
    } catch(std::runtime_error& e) {
        std::cerr << "Could not get username" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 1;
}
