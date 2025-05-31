#include <limits>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "Manager.h"


std::string getVersion(const std::string& package) {
    std::size_t at = package.find('@');
    if (at != std::string::npos) {
        return package.substr(at + 1);
    }
    return "latest";
}

std::string getName(const std::string& package) {
    std::size_t at = package.find('@');
    if (at != std::string::npos) {
        return package.substr(0, at);
    }
    return package;
}



int main(int argc, char* argv[]) {
    std::string basePath = getenv("HOME");
    std::string operation;
    std::string managerName;
    std::vector<std::string> packages;

    if(basePath == "/root") {
	basePath = "/home/" + std::string(getenv("SUDO_USER"));
    }

    basePath = basePath + std::string("/.rqp");

    if(!std::filesystem::exists(basePath)) {
	std::filesystem::create_directories(basePath);
    } else if(!std::filesystem::is_directory(basePath)) {
	std::cerr << "Error: " << basePath << " is not a directory." << std::endl;
	return 1;
    }

    if(!std::filesystem::exists(basePath + "/packages")) {
	std::filesystem::create_directories(basePath + "/packages");
    }

    basePath += "/managers";
    if(!std::filesystem::exists(basePath)) {
	std::filesystem::create_directories(basePath);
    } else if(!std::filesystem::is_directory(basePath)) {
	std::cerr << "Error: " << basePath << " is not a directory." << std::endl;
	return 1;
    }



    if(argc < 2) {
	std::cerr << "Usage: " << argv[0] << " <operation> <manager_name> <package_name>" << std::endl;
	return 1;
    }

    operation = argv[1];

    if(argc < 3) {
	std::cerr << "Usage: " << argv[0] << " <operation> <manager_name> <package_name>" << std::endl;
	return 1;
    }

    managerName = getName(argv[2]);


    if(!std::filesystem::exists(basePath + "/" + managerName)) {
	std::cerr << "Error: Manager '" << managerName << "' does not exist." << std::endl;
	return 1;
    }

    basePath += "/" + managerName;

    if(!std::filesystem::is_symlink(basePath + "/" + getVersion(argv[2]))) {
	std::cerr << "Error: Manager '" << getVersion(argv[2]) << "' is not a symlink." << std::endl;
	return 1;
    }
    basePath = std::filesystem::read_symlink(basePath + "/" + getVersion(argv[2])).string();

    if(argc > 3) {
	for(int i = 3; i < argc; ++i) {
	    packages.push_back(argv[i]);
	}
    }

    Manager manager(basePath);
    manager.init();


    if(operation == "install") {
	manager.install(packages);
    } else if(operation == "remove") {
	manager.remove(packages);
    } else if(operation == "update") {
	manager.update(packages);
    } else if(operation == "list") {
	manager.list();
    } else if(operation == "search") {
	if(packages.size() != 1) {
	    std::cerr << "Usage: " << argv[0] << " search <manager_name> <regex>" << std::endl;
	    return 1;
	}
	manager.search(packages[0]);
    } else if(operation == "info") {
	if(packages.size() != 1) {
	    std::cerr << "Usage: " << argv[0] << " info <manager_name> <package_name>" << std::endl;
	    return 1;
	}
	manager.info(packages[0]);
    } else if(operation == "exists") {
	if(packages.size() != 1) {
	    std::cerr << "Usage: " << argv[0] << " exists <manager_name> <package_name>" << std::endl;
	    return 1;
	}
	manager.exists(packages[0]);
    } else {
	std::cerr << "Unknown operation: " << operation << std::endl;
	return 1;
    }

    return 0;
}
