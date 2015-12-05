## Day 02

### Data pipeline

1. The engine player will scan for data contents in current `app/` and `usr/` folders and related subdirs in every launch.
2. For every source asset, a binary-friendly processed asset will be appended into a big cache/`journal` file.
3. After all new assets are journaled, the player will proceed to normal execution and run the game.
4. Since data is journaled you can revert to a previous version by hitting a few keystrokes in the game editor while running.

Tip: Content in `usr/` folder has higher precedence than journaled data. Put content on this folder to preview local data changes quickly on your computer. No one in your team will be annoyed because of your commits & prototiping anymore.

### Deployment

- Just concat `player+journal` file into a new amalgamated binary, and distribute it as desired.
- This new binary: 1) will not scan for contents on disk ever, 2) will mount appended journal, 3) will start execution as soon as possible.

### Journaling

I've just implemented today a separate library [(journey)](https://github.com/r-lyeh/journey) to do the journaling tech. These are the current features.

- [x] Journaling support: data can be rolled back to an earlier state to retrieve older versions of files.
- [x] Append-only format: create or update new entries just by appending stuff to the journal file.
- [x] Compaction support: all duplicated names are removed and only the one with the largest timestamp is kept.
- [x] Always aligned: data is always aligned for safe accesses.
- [x] Concat friendly: journals can be glued together and the result will still be a valid journey file.
- [x] Foreign support: can append data to a foreign file and result will still be a valid journey file.

I am also adding the [Whereami](https://github.com/gpakosz/whereami) and [Apathy](https://github.com/r-lyeh/apathy) libraries to the repo. The first one is to locate the current executable and the current module, and the second one is a path/file/mstream/mmap IO library. These three libraries together meet the data pipeline tech requeriments. It's only about connecting the wires now.

