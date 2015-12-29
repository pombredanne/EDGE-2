#include "plugin.h"
#include "test.h"

#include <assert.h>
#include <stdexcept>
#include <iostream>
#include <string>

int main() {

	// Set the plugin shared library location
	std::string path("./");
	std::string basename("test");
	std::string ext(".so");

#ifdef _WIN32
#	ifdef _DEBUG
		ext = "d.dll";
#	else
		ext = ".dll";
#	endif
#endif

	std::string dllpath = path + basename + ext;

	using namespace plugin;

	// Load the shared library
	std::cout << "Loading: " << dllpath << std::endl;

	// Call a C function
	auto gimmeFive = dll<int()>( dllpath, "gimmeFive" );
	assert(gimmeFive() == 5);

	// Get plugin descriptor and exports
	PluginDetails* info = sym<PluginDetails*>(dllpath, "exports");

	// API version checking
	if ( (info->apiVersion / 100) != (PLUGIN_API_VERSION / 100) ) {
		std::cerr << "Plugin version mismatch. Expected "
			<< PLUGIN_API_VERSION << ", got " << info->apiVersion;
		return -1;
	}

	std::cout << "plugin-info {" << std::endl;
	std::cout << "\tAPI Version: " << info->apiVersion << std::endl;
	std::cout << "\tFile Name: " << info->fileName << std::endl;
	std::cout << "\tClass Name: " << info->className << std::endl;
	std::cout << "\tClass Base: " << info->classBase << std::endl;
	std::cout << "\tPlugin Version: " << info->pluginVersion << std::endl;
	std::cout << "\tPlugin Author: " << info->pluginAuthor << std::endl;
	std::cout << "\tPlugin License: " << info->pluginLicense << std::endl;
	std::cout << "\tPlugin Dependencies: " << info->pluginDependencies << std::endl;
	std::cout << "}" << std::endl;

	// Instantiate the plugin
	IPlugin* plugin = reinterpret_cast<IPlugin*>(info->initializeFunc());

	// Call string accessor methods
	plugin->setValue("abracadabra");
	assert(strcmp(plugin->cValue(), "abracadabra") == 0);
#if PLUGU_ENABLE_STL
	assert(plugin->sValue() == "abracadabra");
#endif

	// Call command methods
	const void *argvA[] = { "options:set", "randomdata" };
	assert( plugin->rpc( 2, argvA ) );
	assert( plugin->error() == nullptr );

	const void *argvC[] = { "file:write", "hello world", (void *)strlen("hello world") };
	assert( plugin->rpc( 3, argvC ) );

	const void *argvB[] = { "unknown:command", "randomdata" };
	assert( plugin->rpc( 2, argvB ) == false );
	assert( strcmp(plugin->error(), "Unknown command") == 0 );

	std::cout << "Ending" << std::endl;
}
