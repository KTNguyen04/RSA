#include <iostream>
#include "BigIntMod.h"
#include "RSA.h"
using namespace std;
int main()
{
    BigIntMod a, b, c;
    a = "41";
    b = "2";
    // c = "2";
    // RSA::EGCDResult res = RSA::extendedEuclidean(a, b);
    // cout << "gcd: " << res.gcd << endl;
    // cout << "x: " << res.x << endl;
    // cout << "y: " << res.y << endl;

    // cout << "Check: " << a * res.x + b * res.y << endl;

    cout << "a + b = " << (a + b) << endl;
    cout << "a - b = " << (a - b) << endl;
    cout << "a * b = " << (a * b) << endl;
    cout << "a % b = " << (a % b) << endl;
    cout << "a / b = " << (a / b) << endl;
    cout << "b % a = " << (b % a) << endl;
    cout << "a*2 = " << mul2Mod(a, c) << endl;
    return 0;
}