#pragma once

#include "Hasher.h"
#include "Index.h"
#include "IgnoreManager.h"
#include "ObjectStore.h"

#include <unordered_map>

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
    IgnoreManager ignoreManager;
    ObjectStore objectStore;
    
    std::filesystem::path vcsDirectory;

    std::unordered_map<std::filesystem::path, std::string>
getHeadMap() const;

    std::unordered_map<std::filesystem::path, std::string>getWorkingTreeEntries() const;

    std::unordered_map<std::filesystem::path, std::string>
getIndexMap() const;

    void compareStates(
    const std::unordered_map<std::filesystem::path, std::string>& head,
    const std::unordered_map<std::filesystem::path, std::string>& index,
    const std::unordered_map<std::filesystem::path, std::string>& working) const;

public:
    explicit StatusCommand(const std::filesystem::path& vcsDirectory);

    void execute();
};
