#include <Manager.h>


Manager::Manager(const std::string& packageManagerPath) : packageManagerPath(packageManagerPath) {
}

Manager::~Manager() {
}

bool Manager::callFunction(sol::function func) {
	sol::protected_function_result result = func();
	if (!result.valid()) return false;
	return result.get_type() == sol::type::boolean ? result : static_cast<bool>(result);
}

bool Manager::callFunction(sol::function func, const std::string& arg) {
	sol::protected_function_result result = func(arg);
	if (!result.valid()) return false;
	return result.get_type() == sol::type::boolean ? result : static_cast<bool>(result);
}

bool Manager::callFunction(sol::function func, const std::vector<std::string>& args) {
	sol::protected_function_result result = func(args);
	if (!result.valid()) return false;
	return result.get_type() == sol::type::boolean ? result : static_cast<bool>(result);
}


bool Manager::init() {
	this->luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::math, sol::lib::os);
	sol::protected_function_result res = this->luaState.script_file(this->packageManagerPath);

	if (!res.valid()) {
		sol::error err = res;
		std::cerr << "Error loading Lua script: " << err.what() << std::endl;
		return false;
	}
	this->wrapper = res;
	return true;
}

bool Manager::isFunction(const sol::object& obj) {
    return obj.get_type() == sol::type::function;
}

bool Manager::install(const std::vector<std::string> packageNames) {
	sol::function preInstall = this->wrapper["preInstall"];
	sol::function install = this->wrapper["install"];
	sol::function postInstall = this->wrapper["postInstall"];

	if (isFunction(preInstall) && !callFunction(preInstall, packageNames)) {
		std::cerr << "Pre-installation failed." << std::endl;
		return false;
	}

	if (isFunction(install) && !callFunction(install, packageNames)) {
		std::cerr << "Installation failed." << std::endl;
		return false;
	}

	if (isFunction(postInstall) && !callFunction(postInstall, packageNames)) {
		std::cerr << "Post-installation failed." << std::endl;
		return false;
	}

	return true;
}


bool Manager::remove(const std::vector<std::string> packageNames) {
	sol::function preRemove = this->wrapper["preRemove"];
	sol::function remove = this->wrapper["remove"];
	sol::function postRemove = this->wrapper["postRemove"];

	if (isFunction(preRemove) && !callFunction(preRemove, packageNames)) {
		std::cerr << "Pre-removal failed." << std::endl;
		return false;
	}

	if (isFunction(remove) && !callFunction(remove, packageNames)) {
		std::cerr << "Removal failed." << std::endl;
		return false;
	}

	if (isFunction(postRemove) && !callFunction(postRemove, packageNames)) {
		std::cerr << "Post-removal failed." << std::endl;
		return false;
	}

	return true;
}


bool Manager::update(const std::vector<std::string> packageNames) {
	sol::function preUpdate = this->wrapper["preUpdate"];
	sol::function update = this->wrapper["update"];
	sol::function postUpdate = this->wrapper["postUpdate"];

	if (isFunction(preUpdate) && !callFunction(preUpdate, packageNames)) {
		std::cerr << "Pre-update failed." << std::endl;
		return false;
	}

	if (isFunction(update) && !callFunction(update, packageNames)) {
		std::cerr << "Update failed." << std::endl;
		return false;
	}

	if (isFunction(postUpdate) && !callFunction(postUpdate, packageNames)) {
		std::cerr << "Post-update failed." << std::endl;
		return false;
	}

	return true;
}

bool Manager::list() {
	sol::function list = this->wrapper["list"];
	if (!isFunction(list)) {
		std::cerr << "List function not defined." << std::endl;
		return false;
	}
	return callFunction(list);
}

bool Manager::search(const std::string regex) {
	sol::function search = this->wrapper["search"];
	if (!isFunction(search)) {
		std::cerr << "Search function not defined." << std::endl;
		return false;
	}
	return callFunction(search, regex);
}

bool Manager::info(const std::string packageName) {
	sol::function info = this->wrapper["info"];
	if (!isFunction(info)) {
		std::cerr << "Info function not defined." << std::endl;
		return false;
	}
	return callFunction(info, packageName);
}

bool Manager::exists(const std::string packageName) {
	sol::function exists = this->wrapper["exists"];
	if (!isFunction(exists)) {
		std::cerr << "Exists function not defined." << std::endl;
		return false;
	}
	return callFunction(exists, packageName);
}

void Manager::setPackageBasePath(const std::string& path) {
    this->packageBasePath = path;
}
