#include "api.hpp"
#include "app/app.hpp"

int eeModuleGetApplicationPath( std::string &out ) {
	out = get_app_path();
	return 0;
}

int eeModuleGetLibraryPath( std::string &out ) {
	out = get_module_path();
	return 0;
}
