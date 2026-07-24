#pragma once
#include<iostream>
#include<ctime>


class Commit{

private:
    std::string message;
    std::string treeHash;
    std::string parentHash;
    std::time_t timestamp;

public:
    Commit(const std::string& message,
           const std::string& treeHash,
           const std::string& parentHash);

    std::string serialize() const;
};