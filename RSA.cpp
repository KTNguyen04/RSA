#include "RSA.h"
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
        cout << "A: " << A << ", B: " << B << endl;
        cout << "q: " << q << ", r: " << r << endl;

        A = B;
        B = r;

        x2 = x0 - q * x1;
        x0 = x1;
        x1 = x2;

        BigIntMod temp = q * y1;

        y2 = y0 - temp;

        y0 = y1;
        y1 = y2;
        cout << "x0: " << x0 << ", x1: " << x1 << ", x2: " << x2 << endl;
        cout << "y0: " << y0 << ", y1: " << y1 << ", y2: " << y2 << endl;
        cout << "------------------------" << endl;
    }
    return RSA::EGCDResult{A, x0, y0};
}
