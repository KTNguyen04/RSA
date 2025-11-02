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
};