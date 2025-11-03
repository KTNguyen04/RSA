#include <iostream>
#include "BigIntMod.h"
#include "RSA.h"
#include <filesystem>
#include <vector>
#include <fstream>
#include <algorithm>
namespace fs = std::filesystem;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }
    string input_file = argv[1];
    string output_file = argv[2];
    ifstream infile(input_file);
    if (!infile)
    {
        cerr << "Error opening input file: " << input_file << endl;
        return 1;
    }
    BigIntMod N, k, x;
    infile >> N >> k >> x;
    infile.close();
    BigIntMod d = RSA::encrypt_decrypt(x, k, N);
    ofstream outfile(output_file);
    if (!outfile)
    {
        cerr << "Error opening output file: " << output_file << endl;
        return 1;
    }
    outfile << d << endl;
    outfile.close();
    return 0;
}