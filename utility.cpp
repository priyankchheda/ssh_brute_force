#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "utility.h"

std::vector<std::string> readFile(std::string filename)
{
    ifstream inFile;
    inFile.open(filename.c_str());

    if (!inFile) {
        std::cerr << "Unable to open " << filename << "\n";
        exit(1);
    }
    std::vector<std::string> arr;
    std::string word;

    while (inFile >> word)
        arr.push_back(word);

    inFile.close();
    return arr;
}
