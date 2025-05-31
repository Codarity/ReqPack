#include <string>
#include <vector>
#include <limits>
#include "sol/sol.hpp"
#include "ApiBinder.h"


class Manager {
private:
	std::string packageManagerPath;
	std::string packageBasePath;
	std::vector<std::string> dependencies;

	sol::state luaState;
	sol::table wrapper;

	bool isFunction(const sol::object& obj);
	void setPackageBasePath(const std::string& path);
	bool callFunction(sol::function func);
	bool callFunction(sol::function func, const std::string& arg);
	bool callFunction(sol::function func, const std::vector<std::string>& args);

public:

	Manager(const std::string& packageManagerPath);
	~Manager();
	bool init();
	bool install(const std::vector<std::string> packageNames);
	bool remove(const std::vector<std::string> packageNames);
	bool update(const std::vector<std::string> packageNames);
	bool list();
	bool search(const std::string regex);
	bool info(const std::string packageNames);
	bool exists(const std::string packageNames);
};
