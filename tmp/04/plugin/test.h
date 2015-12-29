#pragma once
#include "plugin.h"
#include <string>

namespace plugin {
	// Declare interface class for plugins
	class IPlugin {
	public:

	  IPlugin() {};
	  virtual ~IPlugin() {};

	  //
	  /// Commands

	  virtual bool init() = 0;

	  virtual bool suspend() = 0;
	  virtual bool resume() = 0;

	  virtual bool tick( double t, float dt ) = 0;

	  virtual bool quit() = 0;

	  // Handle a command from the application.
	  virtual bool rpc( int argc, const void **argv ) = 0;

	  // Return the last error message as a char pointer.
	  // If no error is set a nullptr will be returned.
	  virtual const char* error() const = 0;

	  //
	  /// String accessors

	  // Sets the internal string from the given value.
	  // The given value must be null terminated.
	  virtual void setValue(const char* value) = 0;

	  // Return the internal string value as a char pointer.
	  // Since we are returning a POD type plugins will be ABI agnostic.
	  virtual const char* cValue() const = 0;
	};
}

class TestPlugin : public plugin::IPlugin {
public:
	TestPlugin();
	virtual ~TestPlugin();

	virtual bool init() { return true; }

	virtual bool suspend() { return true; }
	virtual bool resume() { return true; }

	virtual bool tick( double t, float dt ) { return true; }

	virtual bool quit() { return true; }

	//
	/// Commands

	// Handle a command from the application.
    virtual bool rpc( int argc, const void **argv );

	// Return the last error message as a char pointer.
	// If no error is set a nullptr will be returned.
	virtual const char* error() const;

	//
	/// String accessors
	virtual void setValue(const char* value);
		// Sets the internal string from the given value.
		// The given value must be null terminated.

	virtual const char* cValue() const;
		// Return the internal string value as a char pointer.
		// Since we are returning a POD type plugins will be ABI agnostic.

protected:
	 std::string _error;
	 std::string _value;
};
