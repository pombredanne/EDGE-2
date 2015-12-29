+---+---+---+---+---+---+---+---+
| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
+---+---+---+---+---+---+---+---+
| s   i   c   a   p |   bits    |
+---+---+---+---+---+---+---+---+ 

type (flags) {
    s : is signed
    i : is integer
    c : is container (if c=1, after declaration follows a number-of-items block)
    a : is associative (requires c=1) (true=map, false=vector)
    p : is pointer
}

-- note on maps: current byte declares key type, and next byte declares value type


  s i c a p | bits
------------+------
[ 0 0 0 0 0 |   0 ] -> void       (non-signed, non-integer, non-pointer, non-associative, non-sequential,   0 bits to hold information)
[ 0 0 0 0 0 |   1 ] -> bool       (non-signed, non-integer, non-pointer, non-associative, non-sequential, 2^1 bits to hold information)
[ 0 0 1 0 0 |   3 ] -> utf8_t     (non-signed, non-integer, non-pointer, non-associative, non-sequential, 2^3 bits to hold information)
[ 0 0 1 0 0 |   4 ] -> utf16_t    (non-signed, non-integer, non-pointer, non-associative, non-sequential, 2^4 bits to hold information)
[ 0 0 1 0 0 |   5 ] -> utf32_t    (non-signed, non-integer, non-pointer, non-associative, non-sequential, 2^5 bits to hold information)
------------+------
[ 0 1 0 0 0 |   3 ] -> uint8_t    (non-signed,     integer, non-pointer, non-associative, non-sequential, 2^3 bits to hold information)
[ 0 1 0 0 0 |   4 ] -> uint16_t   (non-signed,     integer, non-pointer, non-associative, non-sequential, 2^4 bits to hold information)
[ 0 1 0 0 0 |   5 ] -> uint32_t   (non-signed,     integer, non-pointer, non-associative, non-sequential, 2^5 bits to hold information)
[ 0 1 0 0 0 |   6 ] -> uint64_t   (non-signed,     integer, non-pointer, non-associative, non-sequential, 2^6 bits to hold information)
[ 0 1 0 0 0 |   7 ] -> uint128_t  (non-signed,     integer, non-pointer, non-associative, non-sequential, 2^6 bits to hold information)
[ 1 1 0 0 0 |   3 ] -> int8_t     (    signed,     integer, non-pointer, non-associative, non-sequential, 2^3 bits to hold information)
[ 1 1 0 0 0 |   4 ] -> int16_t    (    signed,     integer, non-pointer, non-associative, non-sequential, 2^4 bits to hold information)
[ 1 1 0 0 0 |   5 ] -> int32_t    (    signed,     integer, non-pointer, non-associative, non-sequential, 2^5 bits to hold information)
[ 1 1 0 0 0 |   6 ] -> int64_t    (    signed,     integer, non-pointer, non-associative, non-sequential, 2^6 bits to hold information)
[ 1 1 0 0 0 |   7 ] -> int128_t   (    signed,     integer, non-pointer, non-associative, non-sequential, 2^6 bits to hold information)
------------+------
[ 1 0 0 0 0 |   3 ] -> micro      (    signed, non-integer, non-pointer, non-associative, non-sequential, 2^3 bits to hold information)
[ 1 0 0 0 0 |   4 ] -> half       (    signed, non-integer, non-pointer, non-associative, non-sequential, 2^4 bits to hold information)
[ 1 0 0 0 0 |   5 ] -> float      (    signed, non-integer, non-pointer, non-associative, non-sequential, 2^5 bits to hold information)
[ 1 0 0 0 0 |   6 ] -> double     (    signed, non-integer, non-pointer, non-associative, non-sequential, 2^6 bits to hold information)
------------+------

[ 0 0 0 0 1 |   0 ] -> void*                   (non-signed, non-integer,     pointer, non-associative, non-sequential,   0 bits to hold information)
[ 0 0 1 0 1 |   1 ] -> std::vector<bool*>      (non-signed, non-integer,     pointer, non-associative, non-sequential, 2^1 bits to hold information)

[ 1 1 1 1 1 |   5 ] [ 0 0 1 0 0 |  5 ] [ 00 00 00 x80 ] -> std::map<int32_t, utf32_t> map( 80 );
