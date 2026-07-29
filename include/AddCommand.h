#pragma once

#include<filesystem>

#include "Hasher.h"
#include "ObjectStore.h"
#include "Index.h"

/**
 * @class AddCommand
 * @brief Implements the add command.
 *
 * Reads a file from the working directory,
 * stores its contents as a blob object,
 * and stages it in the index.
 */


class AddCommand{

private:
    Hasher hasher;
    ObjectStore objectStore;
    Index  index;

public:

    /**
     * Creates an AddCommand for the given repository.
     */
    explicit AddCommand(const std::filesystem::path& vcsPath);
    
    /**
     * Stages a file for the next commit.
     *
     * The file is hashed, stored as a blob,
     * and recorded in the staging index.
     */
    void execute(const std::filesystem::path& filePath);
};

