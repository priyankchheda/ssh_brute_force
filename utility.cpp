#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "utility.h"
#include "spdlog/spdlog.h"

std::vector<std::string> readFile(std::string filename)
{
    auto logger = spdlog::get("logger");
    std::ifstream inFile;
    inFile.open(filename.c_str());

    if (!inFile) {
        logger->error("unable to open {}", filename);
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
