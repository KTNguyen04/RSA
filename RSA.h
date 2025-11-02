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
    static BigIntMod inverseMod(const BigIntMod &a, const BigIntMod &m);
    static bool primeTest(const BigIntMod &n);

private:
    static string pToHex(int p);
};