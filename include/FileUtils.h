#pragma once

#include <filesystem>
#include <string>

/**
 * @class FileUtils
 * @brief Provides utility functions for file operations.
 */
class FileUtils
{
public:
    /**
     * @brief Reads the entire contents of a file.
     *
     * @param filePath Path of the file to read.
     * @return File contents as a string.
     */
    static std::string readFile(const std::filesystem::path& filePath);
};