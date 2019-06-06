#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "utility.h"
using namespace std;

vector<string> readFile(string filename)
{
    ifstream inFile;
    inFile.open(filename.c_str());

    if (!inFile) {
        cerr << "Unable to open " << filename << "\n";
        exit(1);
    }
    vector<string> arr;
    string word;

    while (inFile >> word)
        arr.push_back(word);

    inFile.close();
    return arr;
}
