### Dec 2015

This is my very last engine for the next years, I swear.
I plan to include most of the tech I developed during the previous years, in a modular, scriptable, deployable way.
Made the initial commit and planning. There is no going back.

## Rationale core design (wishlist)
- [ ] Full featured C++ engine with C API interface.
- [ ] Module extendable: both native (dynload C++: .dll/.so) and Lua scriptable (for now).
- [ ] Data-driven: engine as data player (.dll/monolithic binary), game as data.
- [ ] Entity/component/system based.
- [ ] Work together with external editors.
- [ ] Work together with diverse data pipelines.
- [ ] Flexible build model.
- [ ] Multiplatform, with desktop in mind.
- [ ] Tiny in conception, but extendable.
- [ ] Fast building times.
- [ ] Package manager.
- [ ] Unit-tested.
- [ ] ZLIB/LibPNG licensed.

## Well thought anti-features
- [ ] Very basic (and replaceable) rendering, audio, input, network and UI classes.
- [ ] Users will provide implementations to meet their taste, hopefully.

## Repository structure
- [:open_file_folder:&nbsp;/](/), readmes and scripts.
- [:open_file_folder:&nbsp;/3rd/](/3rd), third-party libraries.
- [:open_file_folder:&nbsp;/app/](/app), where target executables are written.
- [:open_file_folder:&nbsp;/dev/](/dev), where tools are located. some of them might be compiled during the build process as well.
- [:open_file_folder:&nbsp;/doc/](/doc), where documentation and blog posts are located.
- [:open_file_folder:&nbsp;/img/](/img), where screenshots and videos are written.
- [:open_file_folder:&nbsp;/prj/](/prj), where projects and build scripts are located.
- [:open_file_folder:&nbsp;/src/](/src), where engine sources are located.
- [:open_file_folder:&nbsp;/tut/](/tut), where diverse tutorials are located.

## Building
```
echo clone repo   && git clone https://github.com/r-lyeh/EDGE && cd EDGE
echo posix builds && prj/premake5 gmake  && make -j
echo win32 builds && prj\premake5 vs2013 && msbuild ee.sln /m /p:FS=true /verbosity:minimal
```

## Build model trade-off
▼targets\defines▶ | HAS_DEBUGINFO | HAS_OPTIMIZATIONS | MASTER | C runtime
--- | --- | --- | --- | ---
**DEBUG** | 3 :heavy_check_mark::heavy_check_mark::heavy_check_mark: | 0 :heavy_multiplication_x: | 0 :heavy_multiplication_x: | dynamic
**DEBUGOPT** | 2 :heavy_check_mark::heavy_check_mark: | 1 :heavy_check_mark: | 0 :heavy_multiplication_x:  | static
**RELEASE**/**NDEBUG** | 1 :heavy_check_mark: | 2 :heavy_check_mark::heavy_check_mark: | 0 :heavy_multiplication_x:  | dynamic
**SHIPPING** | 0 :heavy_multiplication_x: | 3 :heavy_check_mark::heavy_check_mark::heavy_check_mark: | 1 :heavy_check_mark: | static

## Licenses
- EDGE, [ZLIB/LibPNG license](LICENSE)

## Alternatives
- [openframeworks.cc](http://openframeworks.cc/)
- [love2d](https://love2d.org/)
- [urho3d](http://urho3d.github.io/)

## Changelog
- v0.0.0 (2015/12/05): Initial commit

