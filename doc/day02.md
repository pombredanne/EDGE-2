## Day 02

### Data pipeline

The data pipeline is in charge of converting the original raw assets into engine-friendly assets. Many times is in charge of packaging and deploying the build too. It is often built in Ruby, Python, Lua or bash files.

EDGE embeds the data pipeline in C++ to simplify player usage and binary redistribution. When EDGE player is launched:

1. `app/`, `res/` and `usr/` folders and related subdirs will be scanned for new content in every launch.
2. For every new/modified source asset, a binary-friendly processed asset will be appended into a big cache/journal `folder.joy` file.
3. After all new assets are journaled, the player will mount all journaled files and run the game.

Tip: You can revert data to a previous version by hitting a few keystrokes in the game editor while running (as long as the journal has not been compacted).

Tip: Content in `usr/` folder has higher precedence than journaled data. This is open for quick prototyping, mods and local temporary changes.

### Deployment

- Just concat `player+folder.joy...` files into a new amalgamated player, and distribute it as desired. This new player will...

1. mount existing appended journal and ignore the `app/` and `res/` folders.
2. scan & mount `usr/` folder only (mods anyone?),
3. run the game.

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
- [Whereami](https://github.com/gpakosz/whereami), to locate paths of the current executable and the current module. Wrote a [tiny C++ wrapper here](../src/app)

These three libraries together meet the data pipeline tech requeriments. It's only about connecting the wires now. Something similar to:

```c++
int main() {
	if( this_app_binary().is_journal_file() ) {
		filesystem.mount( this_app_binary() );
	} else {
		auto globbed = disk_glob_recursive( current_dir(), get_supported_mimes() );
		for( file : globbed ) append_into_journal( "folder.joy", file );
		filesystem.mount( "folder.joy" );
	}

	init();
	game();
	quit();
}
```

### Unified Identifiers

This is a totally biased opinion, but I rather like to keep my asset identifiers locked inmutable in code and forget about them during development. To achieve this, we are using [Unify](https://github.com/r-lyeh/unify).

- [Unify](https://github.com/r-lyeh/unify), to unify game asset names to unique identifiers in a portable way.

We feed Unify all existing asset paths, which get transformed into unified IDs. Even if the team keep changing paths and names on disk, the new paths will converge to the original IDs with a high degree of success. This lead to less code interruptions and provides a much more flexible pipeline.

The virt2phys disk interface is very similar to:

```c++
struct disk {
    map< string, string > table;
    void add( string pathfile ) {
        table[ unify(uri) ] = uri;
    };
    string lookup( string asset ) const {
        return table[ unify(asset) ];
    };
};
```
