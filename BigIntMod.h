#ifndef BIGINTMOD_H
#define BIGINTMOD_H

#include <iostream>
#include <deque>
#include <string>
#include <stdint.h>
#include <array>

using namespace std;

class BigIntMod
{
    deque<int64_t> bits; // most significant bit to least significant bit
    bool sign_;

public:
    BigIntMod();
    // Constructor from string of hexadecimal number store in least significant nibble to most significant nibble left to right
    BigIntMod(const string &);
    static array<int64_t, 4> hexToBits(char hex);
    static char bitsToHex(const deque<int64_t> &bits, size_t pos);

    BigIntMod operator=(const BigIntMod &);
    BigIntMod operator=(const string &); // from hex string
    BigIntMod &operator+=(const BigIntMod &);
    BigIntMod &operator-=(const BigIntMod &);
    BigIntMod &operator*=(const BigIntMod &);
    BigIntMod &operator/=(const BigIntMod &);
    BigIntMod &operator%=(const BigIntMod &);

    friend BigIntMod pow(const BigIntMod &, const BigIntMod &);
    friend BigIntMod pow(const BigIntMod &, const int);

    friend BigIntMod operator+(const BigIntMod &, const BigIntMod &);
    friend BigIntMod operator-(const BigIntMod &, const BigIntMod &);
    friend BigIntMod operator*(const BigIntMod &, const BigIntMod &);
    friend BigIntMod operator/(const BigIntMod &, const BigIntMod &);
    friend BigIntMod operator%(const BigIntMod &, const BigIntMod &);

    friend bool operator>(const BigIntMod &, const BigIntMod &);
    friend bool operator>=(const BigIntMod &, const BigIntMod &);
    friend bool operator<(const BigIntMod &, const BigIntMod &);
    friend bool operator<=(const BigIntMod &, const BigIntMod &);

    friend bool operator==(const BigIntMod &, const BigIntMod &);
    friend bool operator!=(const BigIntMod &, const BigIntMod &);

    friend istream &operator>>(std::istream &, BigIntMod &);
    friend ostream &operator<<(std::ostream &, const BigIntMod &);

    // Modular arithmetic with "trick"

    friend BigIntMod mul2Mod(const BigIntMod &a, const BigIntMod &m);
    friend BigIntMod addMod(const BigIntMod &a, const BigIntMod &b, const BigIntMod &m);
    friend BigIntMod subMod(const BigIntMod &a, const BigIntMod &b, const BigIntMod &m);
    friend BigIntMod mulMod(const BigIntMod &a, const BigIntMod &b, const BigIntMod &m);
    friend BigIntMod powMod(const BigIntMod &base, const BigIntMod &exponent, const BigIntMod &m);
    friend BigIntMod invMod(const BigIntMod &a, const BigIntMod &m);

    // //
    // friend EGCDResult extendedEuclideanMod(const BigIntMod &a, const BigIntMod &b);
    void trim(); // remove useless zero value
    bool isZero() const;
};

#endif