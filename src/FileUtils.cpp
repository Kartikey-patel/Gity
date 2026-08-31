#include "FileUtils.h"
#include<iostream>
#include<fstream>


std::optional<std::string> FileUtils::readFile(const std::filesystem::path& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filePath << '\n';
        return std::nullopt;
    }

    std::string content {
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };

    return content;
}


std::vector<std::filesystem::path>FileUtils::getFilesRecursively(const std::filesystem::path& root,const IgnoreManager& ignoreManager)
{
    std::vector<std::filesystem::path> files;

    auto projectRoot = std::filesystem::current_path();

    for (auto it =
             std::filesystem::recursive_directory_iterator(root);
         it != std::filesystem::recursive_directory_iterator();
         ++it)
    {
        const auto& entry = *it;

        auto relativePath =
            std::filesystem::relative(
                entry.path(),
                projectRoot);

        if (entry.is_directory())
        {
            const auto name = entry.path().filename();

            if (name == ".gity" ||
                name == ".git" ||
                ignoreManager.isIgnored(relativePath))
            {
                it.disable_recursion_pending();
            }

            continue;
        }

        if (!entry.is_regular_file() ||
            ignoreManager.isIgnored(relativePath))
        {
            continue;
        }

        files.push_back(relativePath);
    }

    return files;
}