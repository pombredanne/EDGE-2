## Day 06

### Messages

Messages are handy and become a powerful game pattern. They can be logged and executed later in a render pipeline and inspect the results step by step. Or they could be an scripted action in a SCUMM type graphic adventure. When logged, they can be used to undo/redo actions as well.

Messages are often streamed to disk, but can be streamed to remote systems in a network as well (see RPC services or multiplayer architecture).

We define a message as operation code (opcode) plus data payload. The message will be encapsulated as pair of `(int argc,char **argv)` arguments (similar to `main()` arguments), being `argv[0]` the message opcode and `argv[1]..argv[argc]` the payload.

```c++
struct message {
    int argc;
    char **argv;
    char *opcode = argv[0];
}
message nop  = { 1, { "nop" } };
message add2 = { 2, { "add2", itoa(2), itoa(2) } }
```

### Message pipeline

This is my current proposal for the message pipeline. Any of the stages in between are optional and can be commented out in pairs.

```
  flattering >->   batching >->   compression >-> encryption >-> encoding >-> fragmenting >-> transport send v-v
deflattering <-< debatching <-< decompression <-< decryption <-< decoding <-<    recovery <-< transport recv <-<
```

### De/flattering

Serialization or flattering is the process of converting in-memory data structures into a flat buffer. This is what msgpack or json libraries do.

### De/batching

It is very useful to group many messages in a single buffer together before performing slow i/o operations on them. This is often called batching.
We will be appending messages to a batch by using two techniques:

- On text streams, we will convert the string to a [netstring](https://en.wikipedia.org/wiki/Netstring). Netstrings are a handy tech from the past. Text is just appended like `buf += sprintf( buf, "%d:%s," strlen(str), str);`.
- On binary streams, we will use a similar technique. A msgpack(integer size) number will be always prepended to the message before glueing `[numbytes][stream1]+[numbytes][stream2]+...`.

### De/compression

We can compress the whole buffer and gain some bits. Based off my experience in [bundle compression library](https://github.com/r-lyeh/bundle), here are my favourite compressors:

- [ ] fast: LZJB, CRUSH and SHRINKER. Tiny embeddable sources and reasonable (low) compression ratios (*)
- [ ] good: MINIZ (deflate). A good old technology and an average choice.
- [ ] best: BSC, LZMA. Good choices if targeting desktops and/or the data to unpack is not very large.
- [ ] uber: ZPAQ, BSC, MCM. Best compression ratios, slow times. Ok when you have to decompress only once (ie, in game installers).

(*): With some more integration work, ZSTDF is probably a much better choice, though.

I am choosing CRUSH for now.

- On binary streams, a msgpack(uncompressed size) will be prepended, or msgpack(0) if no compression was done.

### Hashing/verifying

Some compressors will detect if there is something wrong while unpacking, but others wont. So it would be nice to have a checksum for the whole lot as well, for those compressors that do not verify decompressed streams at all. [xxhash](https://github.com/Cyan4973/xxHash) and [murmur3a](https://en.wikipedia.org/wiki/MurmurHash) are among the fastest algorithms, and [FNV1a](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) and [CRC32](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) are among the smallest implementations. There are many others as well: CRC64, GCRC, RS, JS, PJW, ELF, BKDR, SBDM, DJB, DJB2, BP, FNV, AP, BJ1, MH2, SHA1, SFH, etc... I am choosing CRC32 for now.

- On binary streams, a msgpack(crc32) will be prepended.

### En/decryption

Whenever we need to en/decrypt the datas we can use [RC4](https://en.wikipedia.org/wiki/RC4), or any of its variants. I ported an online implementation sometime ago [here](https://github.com/r-lyeh/vault). The implementation is tiny and it is enough for us right now, even if I might revamp the code someday too. Encryption can change the stream length, so it must be specified too.

- On binary streams, a msgpack(length) will be prepended.

### En/decoding (binary-to-text transcoding)

At this point, if we have passed through compression or encryption we are likely to have data in binary content, no matter whether we are in a text or binary stream.

Whenever we need to put binary data (back) in a text stream we can use a base64 encoder. But this data could be embedded in a JSON document in later step, and that would require to escape/unescape quotes too, and parsing would become very inefficent. So we will be using [base91](https://github.com/r-lyeh/base91) instead, which is a tighter encoder and still JSON and XML friendly. Encoded data can be just embedded in a JSON or XML document with no escaping at all. And if we are just in the mood for some day, the repository contains a base64 implementation too.

```
json:
{"blob":"LVZ-|-IIrR\EIp}iTPLgYA"}
```

### Fragmenting/recovery

The message stream is almost ready to be transported, but it needs to be fragmented into packets first. It would be nice if we could fragment and recover them back too, no matter what order they come in. Some transport protocols (like TCP) will do this for us, and others wont (like UDP). Also different protocols have different [MTUs](https://en.wikipedia.org/wiki/Maximum_transmission_unit). This value will have to be adjusted carefully later on, but for now I want prototype quickly: packet size will be 256 - packet control bytes.

