#include "AddCommand.h"
#include "FileUtils.h"
#include <iostream>
#include<fstream>
#include <iterator>

AddCommand::AddCommand(const std::filesystem::path& vcsPath)
    :objectStore(vcsPath / "objects"),
    index(vcsPath / "index")
{
}

void AddCommand::execute(const std::filesystem::path& filePath){
    FileUtils fileUtils;
    std::string contents = fileUtils.readFile(filePath);

    // Generate a unique identifier for the file contents.
    std::string hash = hasher.sha1(contents);

    // Store the file as a blob object.
    objectStore.storeObject(hash,contents);

    std::filesystem::path normalized = filePath.lexically_normal();

    // Record the file in the staging index.
    index.addEntry(hash,normalized);
}