# BigIntMod

A C++ library for manipulating large base-2 integers (exceeding 64 bits), designed for use with the RSA cryptosystem (which requires high speed operations) in the ring $Z_{n=p*q}$.

# Prerequisites

- C++17
- g++13

# Usage

```sh
g++ -g ./main.cpp ./BigIntMod.cpp ./RSA.cpp  -o main
./main <test-file-name> <output-file-name>
```

## References

https://github.com/maemual/BigInt

## Number format

| Format          | Example 1                | Example 2                |
| --------------- | ------------------------ | ------------------------ |
| Hex (normal)    | `0x3A1F`                 | `3A1F`                   |
| Binary (normal) | `0b 0011 1010 0001 1111` | `0011 1010 0001 1111`    |
| Per nibble      | `3, A, 1, F`             | `0011, 1010, 0001, 1111` |
| Input           | `F1A3`                   | —                        |
| Stored in deque | `1111 1000 0101 1100`    | —                        |
| Output          | `F1A3`                   | —                        |
