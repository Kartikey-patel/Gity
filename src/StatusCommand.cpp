#include "StatusCommand.h"
#include "FileUtils.h"
#include "Hasher.h"
#include "Commit.h"
#include "Tree.h"

#include<set>
#include<iostream>
#include<fstream>

StatusCommand::StatusCommand(const std::filesystem::path& vcsDirectory)
    :   index(vcsDirectory/ "index"),
        vcsDirectory(vcsDirectory),
        ignoreManager(vcsDirectory.parent_path()),
        objectStore(vcsDirectory / "objects")
{
}

void StatusCommand::execute()
{
    std::cout << "On branch main\n\n";

    auto head = getHeadMap();
    auto index = getIndexMap();
    auto working = getWorkingTreeEntries();

    compareStates(head, index, working);
}

std::unordered_map<std::filesystem::path, std::string> StatusCommand::getHeadMap()const
{
    std::ifstream headFile(vcsDirectory / "HEAD");

    if (!headFile.is_open())
    {
        return {};
    }

    std::string line;
    std::getline(headFile, line);

    if (line.rfind("ref: ", 0) != 0)
    {
        return {};
    }

    std::filesystem::path refPath = line.substr(5);

    std::ifstream refFile(vcsDirectory / refPath);

    if (!refFile.is_open())
    {
        return {};
    }

    std::string commitHash;
    std::getline(refFile, commitHash);

    if (commitHash.empty())
    {
        return {};
    }

    const std::string commitData =
        objectStore.loadObject(commitHash);

    if (commitData.empty())
    {
        return {};
    }

    const std::string treeHash =
        Commit::getTreeHash(commitData);

    if (treeHash.empty())
    {
        return {};
    }

    const std::string treeData =
        objectStore.loadObject(treeHash);

    if (treeData.empty())
    {
        return {};
    }

    std::unordered_map<std::filesystem::path, std::string> entries;

    for (const auto& entry : Tree::deserialize(treeData))
    {
        entries[entry.filePath] = entry.hash;
    }

    return entries;
}


std::unordered_map<std::filesystem::path, std::string>StatusCommand::getWorkingTreeEntries() const
{
    std::unordered_map<std::filesystem::path, std::string> entries;

    auto workingFiles =
        FileUtils::getFilesRecursively(
            std::filesystem::current_path(),
            ignoreManager);

    for (const auto& filePath : workingFiles)
    {
        auto fullPath =
            std::filesystem::current_path() / filePath;

        auto contents = FileUtils::readFile(fullPath);

        if (!contents)
        {
            continue;
        }

        std::string hash = hasher.sha1(*contents);

        entries[filePath] = hash;
    }

    return entries;
}

std::unordered_map<std::filesystem::path, std::string>StatusCommand::getIndexMap() const
{
    std::unordered_map<std::filesystem::path, std::string> entries;

    for (const auto& entry : index.getEntries())
    {
        entries[entry.filePath] = entry.hash;
    }

    return entries;
}


void StatusCommand::compareStates(const std::unordered_map<std::filesystem::path, std::string>& head,const std::unordered_map<std::filesystem::path, std::string>& index,const std::unordered_map<std::filesystem::path, std::string>& working) const
{
    std::set<std::filesystem::path> allPaths;

    for (const auto& [path, hash] : head)
    {
        allPaths.insert(path);
    }

    for (const auto& [path, hash] : index)
    {
        allPaths.insert(path);
    }

    for (const auto& [path, hash] : working)
    {
        allPaths.insert(path);
    }

    std::set<std::filesystem::path> stagedModified;
    std::set<std::filesystem::path> stagedNew;
    std::set<std::filesystem::path> stagedDeleted;

    std::set<std::filesystem::path> unstagedModified;
    std::set<std::filesystem::path> unstagedDeleted;

    std::set<std::filesystem::path> untracked;

    for (const auto& path : allPaths)
    {
        auto headIt = head.find(path);
        auto indexIt = index.find(path);
        auto workingIt = working.find(path);

        bool inHead = headIt != head.end();
        bool inIndex = indexIt != index.end();
        bool inWorking = workingIt != working.end();

        /*
         * HEAD -> INDEX
         */

        // Existing file modified and staged.
        if (inHead && inIndex && headIt->second != indexIt->second)
        {
            stagedModified.insert(path);
        }

        // New file staged.
        if (!inHead && inIndex && inWorking)
        {
            stagedNew.insert(path);
        }

        // Existing file deleted and deletion staged.
        if (inHead && !inIndex && !inWorking)
        {
            stagedDeleted.insert(path);
        }

        /*
         * INDEX -> WORKING TREE
         */

        // File modified after being staged.
        if (inIndex && inWorking && indexIt->second != workingIt->second)
        {
            unstagedModified.insert(path);
        }

        // Tracked file deleted from working tree.
        if (inHead && inIndex && !inWorking)
        {
            unstagedDeleted.insert(path);
        }

        // New staged file was deleted again.
        if (!inHead && inIndex && !inWorking)
        {
            stagedNew.insert(path);
            unstagedDeleted.insert(path);
        }

        /*
         * Completely new file.
         */

        if (!inHead && !inIndex && inWorking)
        {
            untracked.insert(path);
        }
    }

    bool hasStagedChanges = !stagedModified.empty() || !stagedNew.empty() || !stagedDeleted.empty();

    bool hasUnstagedChanges = !unstagedModified.empty() || !unstagedDeleted.empty();

    /*
     * Print staged changes.
     */

    if (hasStagedChanges)
    {
        std::cout << "Changes to be committed:\n";

        for (const auto& path : stagedNew)
        {
            std::cout << "  new file: " << path << '\n';
        }

        for (const auto& path : stagedModified)
        {
            std::cout << "  modified: " << path << '\n';
        }

        for (const auto& path : stagedDeleted)
        {
            std::cout << "  deleted: " << path << '\n';
        }

        std::cout << '\n';
    }

    /*
     * Print unstaged changes.
     */

    if (hasUnstagedChanges)
    {
        std::cout << "Changes not staged for commit:\n";

        for (const auto& path : unstagedModified)
        {
            std::cout << "  modified: " << path << '\n';
        }

        for (const auto& path : unstagedDeleted)
        {
            std::cout << "  deleted: " << path << '\n';
        }

        std::cout << '\n';
    }

    /*
     * Print untracked files.
     */

    if (!untracked.empty())
    {
        std::cout << "Untracked files:\n";

        for (const auto& path : untracked)
        {
            std::cout << "  " << path << '\n';
        }

        std::cout << '\n';
    }

    /*
     * Clean repository.
     */

    if (!hasStagedChanges && !hasUnstagedChanges && untracked.empty())
    {
        std::cout << "nothing to commit, working tree clean\n";
    }
}