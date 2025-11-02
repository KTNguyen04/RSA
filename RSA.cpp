#include "RSA.h"
#include <random>
#include <sstream>
#include <algorithm>
const BigIntMod ONE = BigIntMod("1");
const BigIntMod TWO = BigIntMod("2");
const BigIntMod ZERO = BigIntMod("0");

RSA::EGCDResult RSA::extendedEuclidean(const BigIntMod &a, const BigIntMod &b)
{
    BigIntMod x0 = ONE, x1 = ZERO;
    BigIntMod y0 = ZERO, y1 = ONE;
    BigIntMod x2, y2;
    BigIntMod A = a, B = b;
    while (!B.isZero())
    {
        BigIntMod q = A / B;
        BigIntMod r = A % B;

        A = B;
        B = r;

        x2 = x0 - q * x1;
        x0 = x1;
        x1 = x2;

        BigIntMod temp = q * y1;
        y2 = y0 - temp;
        y0 = y1;
        y1 = y2;
    }
    return RSA::EGCDResult{A, x0, y0};
}

BigIntMod RSA::inverseMod(const BigIntMod &a, const BigIntMod &m)
{
    EGCDResult res = extendedEuclidean(a, m);
    if (res.gcd != ONE)
    {
        throw std::invalid_argument("Inverse does not exist");
    }
    else
    {
        BigIntMod result = res.x % m;
        if (result < ZERO)
            result += m;
        return result;
    }
}

bool RSA::primeTest(const BigIntMod &n)
{
    if (n <= TWO)
        return (n == TWO);

    if (n.bits[0] == 0) // chẵn
        return false;

    // Step 1: write n-1 = 2^r * m
    BigIntMod m = n - ONE;
    int r = 0;
    while (m.bits[0] == 0) // d even
    {
        m.bits.pop_front(); // d /= 2
        r++;
    }

    auto millerRabinCheck = [&](const BigIntMod &b) -> bool
    {
        BigIntMod x = powMod(b, m, n);
        BigIntMod y;
        for (int i = 1; i <= r; i++)
        {
            y = mulMod(x, x, n);
            if (y == ONE && x != ONE && x != n - ONE)
                return false;
            x = y;
        }
        if (y != ONE)
            return false;
        return true;
    };

    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53};
    int n_primes = sizeof(primes) / sizeof(primes[0]);
    size_t bits = n.bits.size();
    int use_bases = 0;
    if (bits <= 64)
        use_bases = n_primes;
    else if (bits <= 128)
        use_bases = n_primes / 2;
    else if (bits <= 256)
        use_bases = n_primes / 4;
    else
        use_bases = n_primes / 8;

    for (int i = 0; i < use_bases; ++i)
    {

        BigIntMod base(pToHex(primes[i]));
        if (!millerRabinCheck(base))
            return false;
    }

    return true;
}

string RSA::pToHex(int p)
{
    static const char *digits = "0123456789abcdef";
    if (p == 0)
        return "0";
    string s;
    while (p > 0)
    {
        s.push_back(digits[p & 0xF]);
        p >>= 4;
    }

    std::reverse(s.begin(), s.end());
    return s;
}