#include "FileUtils.h"
#include<iostream>
#include<fstream>


std::string FileUtils::readFile(const std::filesystem::path& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filePath << '\n';
        return "";
    }

    return {
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };
}