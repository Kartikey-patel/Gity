#include "CommitCommand.h"
#include "Tree.h"
#include "Hasher.h"
#include "ObjectStore.h"
#include "Commit.h"

#include<fstream>

CommitCommand::CommitCommand(const std::filesystem::path& vcsDirectory)
               : objectStore(vcsDirectory / "objects"),
                 index(vcsDirectory / "index"),
                 vcsDirectory(vcsDirectory)
{
}

std::string CommitCommand::getParentCommitHash()const{
    std::ifstream headFile(vcsDirectory / "HEAD");

    if(!headFile.is_open()){
        return {};
    }
    std::string line;
    std::getline(headFile,line);
    std::filesystem::path refPath = line.substr(5);

    std::ifstream refFile(vcsDirectory / refPath);

    if(!refFile.is_open()){
        return {};
    }
    std::string hash;
    std::getline(refFile,hash);

    return hash;
}

void CommitCommand::execute(const std::string& message){
    auto entries = index.getStagedEntries();

    if (entries.empty())
    {
        std::cout << "Nothing to commit.\n";
        return;
    }

    Tree tree(entries);

    std::string treeData = tree.serialize();
    std::string treeHash = hasher.sha1(treeData);
    objectStore.storeObject(treeHash, treeData);
    std::string parentHash = getParentCommitHash();
    Commit commit(treeHash, parentHash, message);
    std::string commitData = commit.serialize();
    std::string commitHash = hasher.sha1(commitData);
    objectStore.storeObject(commitHash, commitData);
    updateHead(commitHash);
}

void CommitCommand::updateHead(const std::string& commitHash){
    std::ifstream headFile(vcsDirectory / "HEAD");

    if(!headFile.is_open()){
        return;
    }

    std::string line;
    std::getline(headFile,line);
    std::filesystem::path refPath = line.substr(5);

    std::ofstream refFile(vcsDirectory / refPath);
    if(!refFile.is_open()){
        return;
    }
    refFile << commitHash;
}