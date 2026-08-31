#include "StatusCommand.h"
#include "FileUtils.h"
#include "Hasher.h"
#include <vector>
#include<unordered_set>
#include<iostream>

StatusCommand::StatusCommand(const std::filesystem::path& vcsDirectory)
    :   index(vcsDirectory/ "index"),
        vcsDirectory(vcsDirectory),
        ignoreManager(vcsDirectory.parent_path())
{
}


void StatusCommand::printStagedFiles(const std::vector<IndexEntry>& entries){

    if(entries.empty()){
        std::cout << "No staged files.\n";
        return;
    }

    std::cout << "Staged files:\n";
    for(const auto& entry : entries){
        std::cout << " " << entry.filePath.string() << '\n';
    }
}

std::filesystem::path StatusCommand::getWorkingFilePath(const IndexEntry& entry) const
{
    return std::filesystem::current_path() / entry.filePath;
}


void StatusCommand::printModifiedFiles(const std::vector<IndexEntry>& entries){

    bool hasModified = false;
    for(const auto& entry : entries){
        auto filePath = getWorkingFilePath(entry);

        if(!std::filesystem::exists(filePath)){
            continue;
        }
        auto contents = FileUtils::readFile(filePath);
        if(!contents){
            continue;
        }

        const auto currentHash = hasher.sha1(*contents);
        
        if (currentHash != entry.hash){
            if (!hasModified)
            {
                std::cout << "\nModified files:\n";
                hasModified = true;
            }
        
            std::cout << "  " << entry.filePath << '\n';
        }
    }
}


void StatusCommand::printDeletedFiles(const std::vector<IndexEntry>& entries){
    bool hashDeleted = false;
    for(const auto& entry : entries){
        auto filePath = getWorkingFilePath(entry);

        if(!std::filesystem::exists(filePath)){
            if(!hashDeleted){
                std::cout << "\n Deleted files\n";
                hashDeleted = true;
            }
            std::cout << "  " << entry.filePath << '\n';
        }
    }
}




void StatusCommand::printUntrackedFiles(const std::vector<IndexEntry>& entries,const std::vector<std::filesystem::path>& workingFiles){
    std::unordered_set<std::filesystem::path> trackedFiles;
    bool hasUntracked = false;

    for(const auto& entry : entries){
        trackedFiles.insert(entry.filePath);
    }

    for (const auto& file : workingFiles)
    {
        if (trackedFiles.find(file) == trackedFiles.end())
        {
            if (!hasUntracked)
            {
                std::cout << "\nUntracked files:\n";
                hasUntracked = true;
            }

            std::cout << "  " << file << '\n';
        }
    }
}


void StatusCommand::execute()
{
    std::cout << "On branch main\n\n";

    auto entries = index.getEntries();
    auto workingFiles =FileUtils::getFilesRecursively(std::filesystem::current_path(),ignoreManager);

    printStagedFiles(entries);
    printModifiedFiles(entries);
    printDeletedFiles(entries);
    printUntrackedFiles(entries,workingFiles);
}

