#include"Repository.h"
#include<filesystem>
#include<iostream>
#include <fstream>

void Repository::init(){
    auto projectRoot = std::filesystem::current_path();

    // Check if .gity already exist
    if(std::filesystem::exists(projectRoot / ".gity")){
        std::cout <<"Repository already initialized.\n";
        return;
    }

    // Create the main repository directory.
    std::filesystem::create_directory(projectRoot / ".gity");

    auto repositoryPath = projectRoot / ".gity";

    // Create directories used to store objects and branch references.
    std::filesystem::create_directory(repositoryPath / "objects");
    std::filesystem::create_directory(repositoryPath / "refs");

    // Initialize the main branch reference.
    std::ofstream mainRef(repositoryPath / "refs" / "main");
    mainRef.close();
    
    // Initialize HEAD to point to the default branch.
    std::ofstream head(repositoryPath / "HEAD");
    head << "ref: refs/main";
    head.close();

    // Create an empty staging index.
    std::ofstream index(repositoryPath / "index");
    index.close();
    
    std::cout << "Initialized empty VCS repository.\n";
}