#pragma once

#include "ObjectStore.h"
#include<filesystem>

class LogCommand{
private:
    std::filesystem::path vcsDirectory;
    ObjectStore objectStore;
    std::string getCurrentCommitHash() const;
    std::string printCommit(const std::string& commitHash);
    
public:
    explicit LogCommand(const std::filesystem::path& vcsDirectory);

    void execute();
};

