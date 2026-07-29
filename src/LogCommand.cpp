#include "LogCommand.h"
#include<fstream>
#include<iostream>

LogCommand::LogCommand(const std::filesystem::path& vcsDirectory)
    :vcsDirectory(vcsDirectory),
     objectStore(vcsDirectory / "objects")
{
}

std::string LogCommand::getCurrentCommitHash() const{
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
    std::ifstream refFile(vcsDirectory / line);

    if(!refFile.is_open()){
        return "";
    }

    std::string commitHash;
    getline(refFile,commitHash);
    return commitHash;
}

void LogCommand::execute(){
    std::string currentHash = getCurrentCommitHash();

    if (currentHash.empty())
    {
        std::cout << "No commits found.\n";
        return;
    }

    while(!currentHash.empty()){
        currentHash = printCommit(currentHash);
    }
}

std::string LogCommand::printCommit(const std::string& commitHash){
    std::string commitData = objectStore.loadObject(commitHash);

    std::istringstream stream(commitData);

    std::string treeHash;
    std::string parentHash;
    std::string message;
    std::string time;

    std::string line;

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

    std::cout <<"commit:" << commitHash <<'\n';
    std::cout <<"Date:" << time <<'\n';
    std::cout <<"\n";
    std::cout <<"     " << message <<"\n\n"; 

    return parentHash;
}