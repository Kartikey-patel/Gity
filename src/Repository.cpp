#include"Repository.h"
#include<filesystem>
#include<iostream>
#include <fstream>

void Repository::init(){
    auto projectRoot = std::filesystem::current_path();

    if(std::filesystem::exists(projectRoot / ".vcs")){
        std::cout <<"Repository already initialized.\n";
        return;
    }

    // Create the main repository directory.
    std::filesystem::create_directory(projectRoot / ".vcs");

    auto repositorePath = projectRoot / ".vcs";

    // Create directories used to store objects and branch references.
    std::filesystem::create_directory(repositorePath / "objects");
    std::filesystem::create_directory(repositorePath / "refs");
    
    // Initialize HEAD to point to the default branch.
    std::ofstream head(repositorePath / "HEAD");
    head << "ref: refs/main";
    head.close();

    // Create an empty staging index.
    std::ofstream index(repositorePath / "index");
    index.close();
    
    std::cout << "Initialized empty VCS repository.\n";
}