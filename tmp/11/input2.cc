#include <string>
#include <map>
#include <functional>
#include <vector>

struct variant;
std::map<std::string, variant > bindings;

using chord = std::vector<variant>;

struct variant {
	std::string alias;
	std::function<bool()> fn;
	std::vector<variant> list;

	variant()
	{}

	variant( const std::string &alias ) : alias( alias )
	{}

	template<unsigned N>
	variant( const char (&alias)[N] ) : alias( alias )
	{}

	variant( const chord &list ) : list(list)
	{}

	template<typename FN>
	variant( const FN &fn ) : fn(fn)
	{}

	bool operator()() const {
		if( !list.empty() ) {
			for( auto &item : list ) {
				if( !item() ) {
					return false;
				}
			}
		}
		if( !alias.empty() ) {
			auto found = bindings.find( alias );
			if( found == bindings.end() ) {
				return false;
			}
			return found->second();
		}
		if( fn && !fn() ) {
			return false;
		}
		return true;
	}

	operator bool() const {
		return operator()();
	}

};

#include <iostream>
#include "hyde/hyde.hpp"

int main2() {
	bindings["hello"] = [&]{ return std::cout << "hello" << std::endl, true; };
	bindings["greet"] = [&]{ return std::cout << "greet" << std::endl, true; };
	bindings["alias"] = "hello";

	bindings["chord"] = chord { "hello", "greet", "alias" };

	bindings["chord"]();

	hyde::windows::keyboard keyboard(0);

	bindings["key.esc"] = [&]{ return keyboard.escape.trigger(); };
	bindings["key.w"] = [&]{ return keyboard.w.hold(); };
	bindings["key.s"] = [&]{ return keyboard.s.hold(); };
	bindings["key.a"] = [&]{ return keyboard.a.hold(); };
	bindings["key.d"] = [&]{ return keyboard.d.hold(); };

	bindings["walk"] = "key.w";
	bindings["left"] = "key.a";
	bindings["right"] = "key.d";

	bindings["combo"] = chord { "walk", "left", "right" };

	for( ;; ) {
		keyboard.update();
		if( bindings["combo"] || bindings["key.esc"] ) {
			break;
		}
	}

	return 0;
}

#include "eval/eval.hpp"
#include "eval/calculate.hpp"

int main() {
	hyde::windows::keyboard keyboard(0);

	funcs.insert({"key.esc", func_constant(1)});
	funcs.insert({"key.w", func_constant(2)});

	auto key = [&]( int v ) {
		switch( v ) {
			case 1: return keyboard.escape;
			case 2: return keyboard.w;
		}
		return keyboard.zero;
	};

	funcs.insert({"hold", func_args(1, [&](args_t v) { return key(v[0]).hold(); }) });
	funcs.insert({"trigger", func_args(1, [&](args_t v) { return key(v[0]).trigger(); }) });
	funcs.insert({"idle", func_args(1, [&](args_t v) { return key(v[0]).idle(); }) });
	funcs.insert({"release", func_args(1, [&](args_t v) { return key(v[0]).release(); }) });

	while( 1 ) {
		keyboard.update();
		if( eval("hold(key.esc) or trigger(key.w)")) {
			break;
		}
	}
}