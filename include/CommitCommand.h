#pragma once
#include "Hasher.h"
#include "ObjectStore.h"
#include "Index.h"

/**
 * @class CommitCommand
 * @brief Implements the commit command.
 *
 * Creates a tree from the staging area,
 * generates a commit object,
 * stores it in the object database,
 * and updates HEAD.
 */

class CommitCommand{
private:
    Hasher hasher;
    ObjectStore objectStore;
    Index index;
    std::filesystem::path vcsDirectory;

    /**
     * Returns the hash of the current HEAD commit.
     *
     * Returns an empty string if no commits exist.
     */
    std::string getParentCommitHash() const;

    /**
     * Updates the current branch reference
     * to point to the newly created commit.
     */
    void updateHead(const std::string& commitHash);

public:

    /**
     * Creates a CommitCommand for the repository.
     */
    explicit CommitCommand(const std::filesystem::path& vcsDirectory);

    /**
     * Creates a new commit using the staged files.
     */
    void execute(const std::string& message);
};
