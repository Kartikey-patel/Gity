#pragma once
#include<iostream>
#include<ctime>

/**
 * @class Commit
 * @brief Represents a snapshot in the repository history.
 *
 * A commit stores metadata about a repository state, including
 * the tree it points to, its parent commit, the commit message,
 * and the creation timestamp.
 *
 * Commits are serialized and stored in the object database
 * using their SHA-1 hash.
 */

class Commit{

private:
    std::string message;
    std::string treeHash;
    std::string parentHash;
    std::time_t timestamp;

public:

    /**
     * Creates a new commit object.
     *
     * @param treeHash Hash of the associated tree object.
     * @param parentHash Hash of the previous commit.
     * @param message Commit message describing the change.
     */
    Commit(const std::string& treeHash,
           const std::string& parentHash,
           const std::string& message);

    /**
     * Serializes the commit into a text representation.
     *
     * Format:
     * tree <hash>
     * parent <hash>   (optional)
     * time <timestamp>
     * message <text>
     */
    std::string serialize() const;

    static std::string getTreeHash(const std::string& data);
};