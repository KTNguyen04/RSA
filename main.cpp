#include <iostream>
#include "BigIntMod.h"
#include "RSA.h"
#include <filesystem>
#include <vector>
#include <fstream>
#include <algorithm>
namespace fs = std::filesystem;
using namespace std;

int main()
{
    while (true)
    {
        cout << "Enter a number to test for primality (or 'exit' to quit): ";
        string input;
        cin >> input;
        if (input == "exit")
        {
            break;
        }
        try
        {
            BigIntMod num(input);
            bool is_prime = RSA::primeTest(num);
            if (is_prime)
            {
                cout << input << ": 1" << endl;
            }
            else
            {
                cout << input << ": 0" << endl;
            }
        }
        catch (const std::invalid_argument &e)
        {
            cout << "Invalid input: " << e.what() << endl;
        }
    }
}