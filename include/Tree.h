#pragma once
#include<vector>

#include "Index.h"

/**
 * @class Tree
 * @brief Represents a snapshot of the staged files.
 *
 * A tree object is created from the staging index and stores
 * the hash and path of every staged file. The serialized tree
 * is later referenced by a commit object.
 */

class Tree{
private:
    std::vector<IndexEntry> entries;

public:

    /**
    * Creates a tree from the staged index entries.
    */
    explicit Tree(const std::vector<IndexEntry>& entries);

    /**
     * Serializes the tree into a text representation.
     *
     * Format:
     * <hash> <filepath>
     */
    std::string serialize() const;

};