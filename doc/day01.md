## Day 01

### Project structure

So here is the planned disk structure for the project:

- [:open_file_folder:&nbsp;/](/), readmes and scripts.
- [:open_file_folder:&nbsp;/3rd/](/3rd), where third-party libraries are located.
- [:open_file_folder:&nbsp;/app/](/app), where game sources are located.
- [:open_file_folder:&nbsp;/bin/](/bin), where game datas are written.
- [:open_file_folder:&nbsp;/dev/](/dev), where tools are located. some of them might be compiled during the build process as well.
- [:open_file_folder:&nbsp;/doc/](/doc), where documentation and blog posts are located.
- [:open_file_folder:&nbsp;/img/](/img), where screenshots and videos are written.
- [:open_file_folder:&nbsp;/prj/](/prj), where projects and build scripts are located.
- [:open_file_folder:&nbsp;/res/](/res), where raw game resources are located.
- [:open_file_folder:&nbsp;/src/](/src), where engine sources are located.
- [:open_file_folder:&nbsp;/tut/](/tut), where diverse tutorials are located.
- [:open_file_folder:&nbsp;/usr/](/usr), where local user datas are located.

### Cloning

Cloning a git repo with too many submodules in it may become sloppy, so there are no submodules in the repository for now.
Clone the repo by typing:

```
echo clone repo   && git clone https://github.com/r-lyeh/EDGE && cd EDGE
```

### Makefiles and build system

I could have used [Genie](https://github.com/bkaradzic/genie) or [CMake](https://cmake.org/) for this, but I used [Premake5](https://bitbucket.org/premake/premake-dev/wiki/Home) instead.

The premake5 script needs some refactoring but it works for now.
The premake5 binaries are located in the `prj/` folder.

### Building

```
echo posix builds && prj/premake5 gmake  && make -j
echo win32 builds && prj\premake5 vs2015 && msbuild ee.sln /m /p:FS=true /verbosity:minimal
```

Tip: 64-bit builds are the main supported architecture. 32-bit builds will trigger an error and stop the build on purpose; to generate 32-bit builds, comment out the line from the source.

### Regenerating git.hpp

This `git.hpp` header is used to retrieve status from the current git branch and revision, so it can be printed on the running program. To generate the header type:

```
prj\makegit
```

### Main initialization

I will be using [Frodo](https://github.com/r-lyeh/frodo) to init and deinit the EDGE subsystems in strict order.
Frodo is a ring dependency system that ensures that everything is done properly in order, even on system signals and crashes.
Frodo can also restart the application to a desired ring level, or a full restart.

```c++
int main( int argc, const char **argv ) {
    // ring level #0 subsystem: console and memory
    frodo::ring( 0, { "memory", memory::init, memory::quit } );
    frodo::ring( 0, { "console", console::init, console::quit } );
    // ring level #1 subsystem: opengl
    frodo::ring( 1, { "opengl", opengl::init, opengl::quit } );

    if( frodo::init() ) {
        // app starts here
        // [...]

        // shutdown
        return frodo::quit() ? 0 : -1;
    }
}
```

### Main loop

The main loop tick and rendering is going to be driven thru [Hertz](https://github.com/r-lyeh/hertz). Hertz is a simple library that balances rendering and auto-frameskip depending on the desired framerates. Let's set it to 60Hz for now:

```c++
int gameloop() {
    for(;;) {
        double fps = hertz::lock( 60 /*Hz*/, []{/*logic here*/}, []{/*render here*/} );
    }
    hertz::unlock();
}
```
