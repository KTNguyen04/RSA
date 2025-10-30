# BigIntMod

A C++ library for manipulating large base-2 integers (exceeding 64 bits), designed for use with the RSA cryptosystem (which requires high speed operations) in the ring $Z_{n=p*q}$.

# Prerequisites

- C++17
- g++13
- CMake 3.22 or higher

# Usage

```sh
g++ -g ./main.cpp ./BigIntMod.cpp -o main
./main
```

## References

https://github.com/maemual/BigInt
