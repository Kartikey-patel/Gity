#include "LogCommand.h"
#include<fstream>
#include<iostream>
#include<iomanip>
#include<ctime>

LogCommand::LogCommand(const std::filesystem::path& vcsDirectory)
    :vcsDirectory(vcsDirectory),
     objectStore(vcsDirectory / "objects")
{
}

std::string LogCommand::getCurrentCommitHash() const{

    // Read the current branch reference from HEAD.
    std::ifstream file(vcsDirectory / "HEAD");

    if(!file.is_open()){
        return "";
    }

    std::string line;
    getline(file,line);

    if (line.rfind("ref: ", 0) != 0)
    {
        return "";
    }

    line = line.substr(5);

    // Load the latest commit hash from the branch file.
    std::ifstream refFile(vcsDirectory / line);

    if(!refFile.is_open()){
        return "";
    }

    std::string commitHash;
    getline(refFile,commitHash);
    return commitHash;
}

void LogCommand::execute(){

    // Start traversal from the latest commit.
    std::string currentHash = getCurrentCommitHash();

    if (currentHash.empty())
    {
        std::cout << "No commits found.\n";
        return;
    }

    // Follow the parent chain until the initial commit is reached.
    while(!currentHash.empty()){
        currentHash = printCommit(currentHash);
    }
}

std::string LogCommand::printCommit(const std::string& commitHash){

    // Load the serialized commit object.
    std::string commitData = objectStore.loadObject(commitHash);

    std::istringstream stream(commitData);

    std::string treeHash;
    std::string parentHash;
    std::string message;
    std::string time;

    std::string line;

    // Parse the commit metadata.
    while(std::getline(stream,line)){
        if(line.find("parent") == 0){
            parentHash = line.substr(7);
        }
        else if(line.find("tree") == 0) {
            treeHash = line.substr(5);
        }  
        else if(line.find("message") == 0){
            message = line.substr(8);
        }
        else if(line.find("time") == 0) {
            time = line.substr(5);
        }
    }

    // Display the commit information.
    std::cout <<"commit:" << commitHash <<'\n';
    if(!time.empty()){
        std::time_t timestamp = std::stoll(time);
        std::cout << "Date: " << std::put_time(std::localtime(&timestamp), "%a %b %d %H:%M:%S %Y") << '\n';
    }
    else{
        std::cout << "Date: Unknown";
    }
    std::cout <<"\n";
    std::cout <<"     " << message <<"\n\n"; 

    // Return the parent hash so the caller can
    // continue traversing the commit history.
    return parentHash;
}