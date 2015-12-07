## Day 03

### Serialization
Serialization is the process of converting in-memory data structures into a buffer, that can be streamed to disk or to the network.

### Message Pipeline
This is my current proposal for the message pipeline. Any of the stages in between are optional.
```
  serialization >-->   buffering >-->   compression >--> encryption >--> encoding >--> fragmenting >--> transport send
deserialization <--< debuffering <--< decompression <--< decryption <--< decoding <--<     sorting <--< transport recv
```

### Binary from/to text transcoding

Whenever I need to put binary data in a text document I could use a base64 encoder. But then I would have to escape/unescape quotes too, and parsing would not be so efficent anymore, specially when lots of strings would be involved. So I will be using [base91](https://github.com/r-lyeh/base91) instead, which is a tighter encoder and still JSON and XML friendly. If I am in the mood someday, the repository contains a base64 implementation too.

```
json:
{"blob":"LVZ-|-IIrR\EIp}iTPLgYA"}
```

### Data stream concatenation (text) (binary)

- Binary: An integer number will be always prepended `[num streams] [stream #1] [stream #2] [...]`. And that's all.
- Text: [Netstrings](https://en.wikipedia.org/wiki/Netstring) will be used. They are a lovely tech from the past. Text is just formatted like `buf += sprintf( buf, "%d:%s," strlen(str), str);`. Format will be something nested like `netstring(netstring(str1)+netstr(str2)+netstr(str3));` until I find something better.

### Buffering

### Fragmenting / Numbering

### Encryption

Whenever I need to en/decrypt the datas I will be using [RC4](https://en.wikipedia.org/wiki/RC4) or any of its variants. The implementation is tiny and it is enough for us right now. I ported an online implementation sometime ago [here](https://github.com/r-lyeh/vault). Might revamp the code someday.

### Compression (fast)

LZJB, ZSTD, Crush and Shrinker.

### Compression (good)

