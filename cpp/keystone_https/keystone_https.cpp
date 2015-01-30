#include "keystone/Keystone.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {

    if(!(argc == 5 || argc == 6) ) {
        std::cout << "Usage: " <<std::endl;
        std::cout << "\tkeystone_login_https <URL> <username> <password> <tenantname> [certificatePath (optional)]" << std::endl;
        return 1;
    }
    
    const std::string url(argv[1]);
    const std::string username(argv[2]);
    const std::string password(argv[3]);
    const std::string tenantName(argv[4]);

    keystone::Keystone keystone(url);
    
    try {
        keystone::KeystoneUserInfo info;
        if (argc == 6) {
            const std::string certFileName(argv[5]);
            keystone.setCACertificateFilename(certFileName);
        }
        keystone.login(username, password, tenantName, info);
        std::cout << info.getToken() << std::endl;

        return 0;
    } catch(std::runtime_error& e) {
        std::cerr << "Could not log in" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 1;
}
