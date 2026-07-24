#pragma once
#include "Hasher.h"
#include "ObjectStore.h"
#include "Index.h"

class CommitCommand{
private:
    Hasher hasher;
    ObjectStore objectStore;
    Index index;
    std::filesystem::path vcsDirectory;
    std::string getParentCommitHash() const;
    void updateHead(const std::string& commitHash);

public:
    explicit CommitCommand(const std::filesystem::path& vcsDirectory);

    void execute(const std::string& message);
};
