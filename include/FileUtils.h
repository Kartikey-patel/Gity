#pragma once

#include <filesystem>
#include <string>
#include<optional>
#include <vector>
#include "IgnoreManager.h"

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
   static std::optional<std::string> readFile(const std::filesystem::path& filePath);

   static std::vector<std::filesystem::path>getFilesRecursively(const std::filesystem::path& root,const IgnoreManager& ignoreManager);
};