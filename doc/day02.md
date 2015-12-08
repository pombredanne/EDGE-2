## Day 02

### Data pipeline

1. The engine player will scan for data contents in current `app/`, `res/` and `usr/` folders and related subdirs in every launch.
2. For every source asset, a binary-friendly processed asset will be appended into a big cache/journal file (`.joy` extension).
3. After all new assets are journaled, the player will proceed to normal execution and run the game.
4. Since data is journaled you can revert to a previous version by hitting a few keystrokes in the game editor while running.

Tip: Content in `usr/` folder has higher precedence than journaled data. Put content on this folder to preview local data changes quickly on your computer. No one in your team will be annoyed because of your commits & prototiping anymore.

### Deployment

- Just concat `player+player.joy` file into a new amalgamated binary, and distribute it as desired. This new binary will...
- 1) will mount existing appended journal,
- 2) will scan `usr/` folder only (mods anyone?),
- 3) will start execution as soon as possible.

### Journaling

I have just implemented today a separate library [(journey)](https://github.com/r-lyeh/journey) to do the journaling tech. These are the current features.

- [x] Journaling support: data can be rolled back to an earlier state to retrieve older versions of files.
- [x] Append-only format: create or update new entries just by appending stuff to the journal file.
- [x] Compaction support: all duplicated names are removed and only the one with the largest timestamp is kept.
- [x] Always aligned: data is always aligned for safe accesses.
- [x] Concat friendly: journals can be glued together and the result will still be a valid journey file.
- [x] Foreign support: can append data to a foreign file and result will still be a valid journey file.

I am also adding three libraries in the repository:
- [Apathy](https://github.com/r-lyeh/apathy), to do memory-map, path and file operations in a portable way.
- [Unify](https://github.com/r-lyeh/unify), to unify game asset names to unique identifiers in a portable way.
- [Whereami](https://github.com/gpakosz/whereami), to locate paths of the current executable and the current module. Wrote a [tiny C++ wrapper here](../src/app)

These three libraries together meet the data pipeline tech requeriments. It's only about connecting the wires now. Something similar to:

```c++
int main() {
	if( this_app_binary().is_journey_file() ) {
		game::filesystem.mount( this_app_binary() );
	} else {
		auto globbed = apathy::glob_recursive( current_dir(), get_all_supported_mimes() );
		journal::append_into( "edge.journey", unify(globbed) );
		game::filesystem.mount( "edge.journey" );
	}

	init();
	game();
	quit();
}
```

### MIME types

It is often nice to auto-detect file types, specially when file extensions are wrong or hidden. I stumbled upon [file-type](https://github.com/sindresorhus/file-type), a nice javascript library that detects mime types. I ported it and the new C++ library is found in the [mime/](../src/mime) folder. It basically tells you the extension and mime type for a given byte stream. That easy.

### Watchdog

A watchdog library is that one that notifies you about local changes on the monitored files you are watching. I do not know if I will be using this, but it is always handy, specially when monitoring shaders and other graphic assets.

I created quickly a Watchdog library that builds on top of Apathy. The library can be found in the [watchdog/](../src/watchdog) folder.

I could have used the original library (where I took the inspiration from) [Watchdog library](https://github.com/simongeilfus/Watchdog), or maybe [EFSW](https://bitbucket.org/SpartanJ/efsw) instead.

### Generic data files (text) (binary)

I want the engine to support binary files in SHIPPING builds, and both binary and text-files in DEBUG/DEBUGOPT/RELEASE builds. This is to ensure the loading times are fast in SHIPPING builds, and but still readable/testable yet binary-friendly in any other configuration.

I could write my own text/binary file formats, but I plan the engine to have a backend server which might be not written in C++ after all. And I need the formats to be supported in any major language, just in case. So I have been considering using de-facto standards instead.

For text data, [JSON for modern C++](https://github.com/nlohmann/json) is my candidate (or maybe [json11](https://github.com/dropbox/json11)). [JSON](https://en.wikipedia.org/wiki/JSON) is not as compact as [Lua](http://www.lua.org/), and not as verbose as [XML](https://en.wikipedia.org/wiki/XML), but it is a de-facto standard and can be used pretty much everywhere. Other alternatives are [Lua](http://www.lua.org/) (yes) and [XML](https://en.wikipedia.org/wiki/XML). [Lua](http://www.lua.org/) is one of my favourite formats, very lightweight and compact; as a data solution it is still unknown to most people, but just look at that lovely [luadata library sample](https://github.com/tynril/luadata/). On the other hand, [XML](https://en.wikipedia.org/wiki/XML) is a very extended format but also very verbose and completely crazy for computers. The engine will have to support it at some point, no doubt. I really dislike the spec, too verbose and fat. We will be using [pugixml parser](http://pugixml.org/) when needed, which is a great, fast and tiny library by the way.

For binary data, [msgpack](https://msgpack.org) is my choice. Like JSON, msgpack is almost supported everywhere as well, plus being a very tight binary format too. There are a few implementations, including [the official one](https://msgpack.org). But I have chosen [CMP implementation](https://github.com/camgunz/cmp) for now. Other format alternatives might be [bson](http://bsonspec.org/), [cbor](http://cbor.io/) or [kazuho/zson](https://github.com/kazuho/zson).

```
json:                              msgpack:
{"compact":true,"schema":0}        [82] [A7] compact [C3] [A6] schema [00]
                                    |    |            |    |           |
                                    |    |            |    |           +--> integer 0
                                    |    |            |    +----------> 6-byte string
                                    |    |            +---------------> boolean  true
                                    |    +----------------------------> 7 byte string
                                    +---------------------------------> 2 element-map
```
