## Day 03

### Generic data file formats (text / binary)

I want the engine to support binary files in SHIPPING builds only, and both binary and text-files in DEBUG/DEBUGOPT/RELEASE builds. This is to ensure the loading times are fast in SHIPPING builds, and but still readable/testable yet binary-friendly in any other configuration.

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

### Hashing

Some compressors will detect if there is something wrong while unpacking, but others wont. So it would be nice to have a checksum for the whole lot as well, for those compressors that do not verify decompressed streams at all. [xxhash](https://github.com/Cyan4973/xxHash) and [murmur3a](https://en.wikipedia.org/wiki/MurmurHash) are among the fastest algorithms, and [FNV1a](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) and [CRC32](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) are among the smallest implementations. There are many others as well: CRC64, GCRC, RS, JS, PJW, ELF, BKDR, SBDM, DJB, DJB2, BP, FNV, AP, BJ1, MH2, SHA1, SFH, etc... I am choosing CRC32 for now.

- On binary streams, a msgpack(crc32) will be prepended.

### Encryption

Whenever we need to en/decrypt the datas we can use [RC4](https://en.wikipedia.org/wiki/RC4), or any of its variants. I ported an online implementation sometime ago [here](https://github.com/r-lyeh/vault). The implementation is tiny and it is enough for us right now, even if I might revamp the code someday too. Encryption can change the stream length, so it must be specified too.

- On binary streams, a msgpack(length) will be prepended.

### Compression

We can compress the whole buffer and gain some bits. Based off my experience in [bundle compression library](https://github.com/r-lyeh/bundle), here are my favourite compressors:

- [ ] tiny: LZJB, CRUSH and SHRINKER. Tiny embeddable sources and low (reasonable) compression ratios.
- [ ] fast: MINIZ (deflate). Good old technology and an average choice.
- [ ] fast: ZSTDF. Faster and better compression ratios than ZIP.
- [ ] best: BSC, LZMA. Good choices if targeting desktops and/or the data to unpack is not very large.
- [ ] uber: ZPAQ, BSC, MCM. High compression ratios, very slow times. Ok when you have to decompress only once (ie, in game installers).

I am choosing CRUSH for now.

- On binary streams, a msgpack(uncompressed size) will be prepended, or msgpack(0) if no compression was done.

### Custom data files formats

The following libraries are likely to be used when dealing with custom data files:

- Truetypes: stb_truetype
- Image: stb_image (*), loadpng (PNG), jo_jpeg (JPG)
- Textures: soil or spot.
- Audio: WAV: stb_vorbis (OGG), jo_wav (WAV), musepack (MPC).
- XML: PugiXML.
- JSON: JSON for modern C++.
- MsgPack: CMP.
- SVG: nanosvg.
- INI: mINI.
- CSV, TSV: split data into newlines, split newlines into tokens (comma or tab separated).

### MIME types and file format detection

It is often nice to auto-detect file types, specially when file extensions are wrong or hidden. I stumbled upon [file-type](https://github.com/sindresorhus/file-type), a nice javascript library that detects mime types. I ported it and the new C++ library is found in the [mime/](../src/mime) folder. It basically tells you the extension and mime type for a given byte stream. That easy.
