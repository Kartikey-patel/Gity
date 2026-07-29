#pragma once

#include "ObjectStore.h"
#include<filesystem>

/**
 * @class LogCommand
 * @brief Displays the repository's commit history.
 *
 * Starts from the latest commit referenced by HEAD
 * and traverses the commit chain by following
 * parent hashes.
 */

class LogCommand{
private:
    std::filesystem::path vcsDirectory;
    ObjectStore objectStore;

    /**
     * Returns the hash of the latest commit
     * referenced by HEAD.
     */
    std::string getCurrentCommitHash() const;

    /**
     * Loads and prints a commit object.
     *
     * @param commitHash Hash of the commit to print.
     * @return Parent commit hash.
     */
    std::string printCommit(const std::string& commitHash);
    
public:

    /**
     * Creates a LogCommand for the repository.
     */
    explicit LogCommand(const std::filesystem::path& vcsDirectory);

    /**
     * Prints the commit history,
     * starting from the latest commit.
     */
    void execute();
};

