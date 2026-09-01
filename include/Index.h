#pragma once

#include <vector>
#include <string>
#include <filesystem>

/**
 * @class Index
 * @brief Manages the staging area of the version control system.
 *
 * The index keeps track of files that have been staged using
 * the add command. Each entry stores the file path along with
 * the SHA-1 hash of its current contents.
 */

 /**
 * Represents a single staged file.
 */
struct IndexEntry
{
    std::string hash;                 // SHA-1 hash of the file contents.
    std::filesystem::path filePath;   // Relative path of the staged file.
};

class Index
{
private:
    std::filesystem::path indexPath;

    /**
     * Reads all staged entries from the index file.
     *
     * Each line of the index has the format:
     * <hash> <filepath>
     */
    std::vector<IndexEntry> loadEntries()const;

    /**
     * Writes all staged entries back to the index file.
     *
     * Existing contents are overwritten with the updated staging area.
     */
    void saveEntries(const std::vector<IndexEntry>& entries)const;

public:
    /**
     * Creates an Index object for the given index file.
     */
    explicit Index(const std::filesystem::path& indexPath);

    /**
     * Adds a file to the staging area.
     *
     * If the file already exists in the index,
     * its hash is updated instead of creating
     * a duplicate entry.
     */
    void addEntry(const std::string& hash, const std::filesystem::path& filePath);

    /**
     * Removes a file from the staging index.
     *
     * Used when a tracked file has been deleted from the
     * working tree and that deletion is being staged.
     */
    void removeEntry(const std::filesystem::path& filePath);

    // public getter to access loadEntries
    std::vector<IndexEntry> getEntries()const;

    void clear()const;
};