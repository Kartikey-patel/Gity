#include <iostream>
#include <string>

#include "Repository.h"
#include "AddCommand.h"
#include "CommitCommand.h"
#include "LogCommand.h"
#include "StatusCommand.h"

int main(int argc, char* argv[])
{
    // Dispatch the requested command.
    if (argc < 2)
    {
        std::cout << "Usage:\n";
        std::cout << "  ./vcs init\n";
        std::cout << "  ./vcs add <file>\n";
        std::cout << "  ./vcs commit <message>\n";
        std::cout << "  ./vcs log\n";
        std::cout << "  ./vcs status\n";
        return 1;
    }
 
    std::string command = argv[1];

    std::filesystem::path vcsDirectory = ".vcs";

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
            std::cout << "Usage: vcs add <file>\n";
            return 1;
        }

        AddCommand add(vcsDirectory);
        add.execute(argv[2]);
    }
    // Create a new commit.
    else if (command == "commit")
    {
        if (argc < 3)
        {
            std::cout << "Usage: vcs commit <message>\n";
            return 1;
        }

        CommitCommand commit(vcsDirectory);
        commit.execute(argv[2]);
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
        std::cout << "Unknown command: " << command << '\n';
        return 1;
    }

    return 0;
}