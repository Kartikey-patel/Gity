#pragma once

#include "Hasher.h"
#include "Index.h"

/**
 * @class StatusCommand
 * @brief Displays the current state of the working directory.
 *
 * Compares the working directory with the staging index to
 * identify staged, modified, and untracked files.
 */


class StatusCommand{
private:
    Hasher hasher;
    Index index;
    
    std::filesystem::path vcsDirectory;

    std::filesystem::path getWorkingFilePath(const IndexEntry& entry) const;

    void printStagedFiles(const std::vector<IndexEntry>& entries);

    void printModifiedFiles(const std::vector<IndexEntry>& entries);

    void printUntrackedFiles(const std::vector<IndexEntry>& entries,const std::vector<std::filesystem::path>& workingFiles);

public:
    explicit StatusCommand(const std::filesystem::path& vcsDirectory);

    std::vector<std::filesystem::path> scanWorkingDirectory() const;

    void execute();
};
