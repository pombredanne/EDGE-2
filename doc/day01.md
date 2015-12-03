## Day 01 

### Project structure

So here is the planned disk structure for the project:

- [:open_file_folder:&nbsp;/](/), readmes and scripts.
- [:open_file_folder:&nbsp;/3rd/](/3rd), third-party libraries.
- [:open_file_folder:&nbsp;/app/](/app), where game datas are located.
- [:open_file_folder:&nbsp;/dev/](/dev), where tools are located. some of them might be compiled during the build process as well.
- [:open_file_folder:&nbsp;/doc/](/doc), where documentation and blog posts are located.
- [:open_file_folder:&nbsp;/img/](/img), where screenshots and videos are written.
- [:open_file_folder:&nbsp;/prj/](/prj), where projects and build scripts are located.
- [:open_file_folder:&nbsp;/src/](/src), where engine sources are located.
- [:open_file_folder:&nbsp;/tut/](/tut), where diverse tutorials are located.
- [:open_file_folder:&nbsp;/usr/](/usr), where local user datas are located.

### Cloning

The cloning process becomes sloppy when there are too many submodules in a git repo. 
So there are no submodules in the repository for now.
Clone the repo by typing:

```
echo clone repo   && git clone https://github.com/r-lyeh/EDGE && cd EDGE
```

### Makefiles and build system

I could have used [genie](https://github.com/bkaradzic/genie) or [cmake](https://cmake.org/) for this, but I used [premake5](https://bitbucket.org/premake/premake-dev/wiki/Home) instead.

The premake5 script needs some refactoring but it works for now.
The premake5 binaries are located in the `prj/` folder.

### Building

```
echo posix builds && prj/premake5 gmake  && make -j
echo win32 builds && prj\premake5 vs2013 && msbuild ee.sln /m /p:FS=true /verbosity:minimal
```

Tip: 32-bit builds will trigger a `static_assert` and stop the build on purpose. Comment it out if you want to. This is to ensure that the 64-bit build is always active and in good shape.

### Regenerating git.hpp

This `git.hpp` header is used to retrieve status from the current git branch and revision, so it can be printed on the running program. To generate the header type:

```
prj\makegit
```

### Data pipeline

1. The engine player will scan for data contents in current `app/` and `usr/` folders and related subdirs in every launch.
2. For every source asset, a binary-friendly processed asset will be appended into a big cache/`journal` file.
3. After all new assets are journaled, the player will proceed to normal execution and run the game.
4. Since data is journaled you can revert to a previous version by hitting a few keystrokes in the game editor while running.

Tip: Content in `usr/` folder has higher precedence than journaled data. Put content on this folder to preview local data changes quickly on your computer. No one in your team will be annoyed because of your commits & prototiping anymore.

### Deployment

- Just concat `player+journal` file into a new amalgamated binary, and distribute it as desired.
- This new binary: 1) will not scan for contents on disk ever, 2) will mount appended journal, 3) will start execution as soon as possible.
