#include "AddCommand.h"

#include <iostream>
#include<fstream>
#include <iterator>

AddCommand::AddCommand(const std::filesystem::path& vcsPath)
    :objectStore(vcsPath / "objects"),
    index(vcsPath / "index")
{
}

void AddCommand::execute(const std::filesystem::path& filePath){

    // Read the contents of the file from the working directory.
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filePath << '\n';
        return;
    }

    std::string contents{
    std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>()
    };

    // Generate a unique identifier for the file contents.
    std::string hash = hasher.sha1(contents);

    // Store the file as a blob object.
    objectStore.storeObject(hash,contents);

    std::filesystem::path normalized = filePath.lexically_normal();

    // Record the file in the staging index.
    index.addEntry(hash,normalized);
}