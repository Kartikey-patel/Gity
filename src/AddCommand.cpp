#include "AddCommand.h"
#include "FileUtils.h"

#include <iostream>
#include<fstream>
#include <iterator>

AddCommand::AddCommand(const std::filesystem::path& vcsPath)
    :objectStore(vcsPath / "objects"),
    index(vcsPath / "index"),
    ignoreManager(vcsPath.parent_path())
{
}

void AddCommand::addFile(const std::filesystem::path& filePath){
    FileUtils fileUtils;
    auto contents = fileUtils.readFile(filePath);

    if(!contents){
        return;
    }

    // Generate a unique identifier for the file contents.
    std::string hash = hasher.sha1(*contents);

    // Store the file as a blob object.
    objectStore.storeObject(hash,*contents);

    std::filesystem::path normalized = filePath.lexically_normal();

    // Record the file in the staging index.
    index.addEntry(hash,normalized);
}

bool AddCommand::execute(const std::filesystem::path& filePath)
{
    if (!std::filesystem::exists(filePath))
    {
        std::cerr << "Error: path '" << filePath
                  << "' does not exist.\n";
        return false;
    }

    if (std::filesystem::is_regular_file(filePath))
    {
        auto relativePath =std::filesystem::relative(filePath,std::filesystem::current_path());

        if (ignoreManager.isIgnored(relativePath))
        {
            return true;
        }

        addFile(filePath);
        return true;
    }

    if (std::filesystem::is_directory(filePath))
    {
        auto files = FileUtils::getFilesRecursively(filePath,ignoreManager);

        for (const auto& file : files)
        {
            addFile(file);
        }
        removeDeletedFiles(filePath);

        return true;
    }
    
    return false;
}

void AddCommand::removeDeletedFiles(const std::filesystem::path& scope)
{
    auto entries = index.getEntries();

    std::filesystem::path normalizedScope =scope.lexically_normal();

    for (const auto& entry : entries)
    {
        auto indexedPath = entry.filePath.lexically_normal();

        bool inScope = false;

        if (normalizedScope == ".")
        {
            inScope = true;
        }
        else
        {
            auto scopeString = normalizedScope.string();
            auto pathString = indexedPath.string();

            inScope =indexedPath == normalizedScope ||
                pathString.rfind(scopeString + "/", 0) == 0;
        }

        if (!inScope)
        {
            continue;
        }

        auto workingPath =std::filesystem::current_path() / indexedPath;

        if (!std::filesystem::exists(workingPath))
        {
            index.removeEntry(indexedPath);
        }
    }
}