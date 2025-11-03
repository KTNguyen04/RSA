#include "BigIntMod.h"
class RSA
{

public:
    struct EGCDResult
    {
        BigIntMod gcd;
        BigIntMod x;
        BigIntMod y;
    };
    static EGCDResult extendedEuclidean(const BigIntMod &a, const BigIntMod &b);
    static BigIntMod keyGen(const BigIntMod &p, const BigIntMod &q, const BigIntMod &e);
    static bool primeTest(const BigIntMod &n);
    static BigIntMod gcd(const BigIntMod &a, const BigIntMod &b);
    static BigIntMod encrypt_decrypt(const BigIntMod &x, const BigIntMod &k, const BigIntMod &n);

private:
    static string pToHex(int p);
};