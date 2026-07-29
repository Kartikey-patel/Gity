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

    // Read the current branch reference from HEAD.
    std::ifstream headFile(vcsDirectory / "HEAD");

    if(!headFile.is_open()){
        return {};
    }
    std::string line;
    std::getline(headFile,line);
    std::filesystem::path refPath = line.substr(5);

    // Load the commit hash stored in the branch reference.
    std::ifstream refFile(vcsDirectory / refPath);

    if(!refFile.is_open()){
        return {};
    }
    std::string hash;
    std::getline(refFile,hash);

    return hash;
}

void CommitCommand::execute(const std::string& message){

    // Load all staged files from the index.
    auto entries = index.getStagedEntries();

    if (entries.empty())
    {
        std::cout << "Nothing to commit.\n";
        return;
    }

    // Build and store the tree object.
    Tree tree(entries);

    std::string treeData = tree.serialize();
    std::string treeHash = hasher.sha1(treeData);
    objectStore.storeObject(treeHash, treeData);

    // Retrieve the parent commit referenced by HEAD.
    std::string parentHash = getParentCommitHash();
    Commit commit(treeHash, parentHash, message);

    // Create and store the new commit object.
    std::string commitData = commit.serialize();
    std::string commitHash = hasher.sha1(commitData);
    objectStore.storeObject(commitHash, commitData);

    // Move HEAD to the newly created commit.
    updateHead(commitHash);
}

void CommitCommand::updateHead(const std::string& commitHash){

    // Resolve the current branch from HEAD.
    std::ifstream headFile(vcsDirectory / "HEAD");

    if(!headFile.is_open()){
        return;
    }

    std::string line;
    std::getline(headFile,line);
    std::filesystem::path refPath = line.substr(5);

    // Update the branch to point to the latest commit.
    std::ofstream refFile(vcsDirectory / refPath);
    if(!refFile.is_open()){
        return;
    }
    refFile << commitHash;
}