#include <iostream>
#include <string>

#include "Repository.h"
#include "AddCommand.h"
#include "CommitCommand.h"
#include "LogCommand.h"
#include "StatusCommand.h"

void printUsage(){
    std::cout << "Usage:\n"
              << "  gity init\n"
              << "  gity add <file>...\n"
              << "  gity commit <message>\n"
              << "  gity log\n"
              << "  gity status\n";
}

std::string buildCommitMessage(int argc, char* argv[]){
    std::string message;
    
    for(int i = 2; i < argc; i++){
        if(!message.empty()){
            message += ' ';
        }
        message += argv[i];
    }
    return message;
}

int main(int argc, char* argv[])
{
    // Dispatch the requested command.
    if (argc < 2)
    {
        printUsage();
        return 1;
    }
 
    std::string command = argv[1];

    std::filesystem::path vcsDirectory = ".gity";

    // Initialize a new repository.
    if (command == "init")
    {
        Repository repo;
        repo.init();
    }
    // Stage a file.
    else if (command == "add")
    {
        if (argc < 3)
        {
            std::cerr << "Error: no file specified.\n";
            printUsage();
            return 1;
        }

        AddCommand add(vcsDirectory);
        
        for(int i = 2; i < argc; i++){
            add.execute(argv[i]);
        }
    }
    // Create a new commit.
    else if (command == "commit")
    {
        if (argc < 3)
        {
            std::cerr << "Error: commit message is required.\n";
            printUsage();
            return 1;
        }

        CommitCommand commit(vcsDirectory);
        std::string message = buildCommitMessage(argc, argv);
        commit.execute(message);
    }
    // Display the commit history.
    else if(command == "log")
    {
        LogCommand logCommand(vcsDirectory);
        logCommand.execute();
    }
    else if(command == "status"){
        StatusCommand st(vcsDirectory);
        st.execute();
    }
    else
    {
        std::cerr << "Unknown command: '" << command << "'.\n";
        printUsage();
        return 1;
    }

    return 0;
}