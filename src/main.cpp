#include <iostream>
#include <string>

#include "Repository.h"
#include "AddCommand.h"
#include "CommitCommand.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage:\n";
        std::cout << "  vcs init\n";
        std::cout << "  vcs add <file>\n";
        std::cout << "  vcs commit <message>\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "init")
    {
        Repository repo;
        repo.init();
    }
    else if (command == "add")
    {
        if (argc < 3)
        {
            std::cout << "Usage: vcs add <file>\n";
            return 1;
        }

        AddCommand add(".vcs");
        add.execute(argv[2]);
    }
    else if (command == "commit")
    {
        if (argc < 3)
        {
            std::cout << "Usage: vcs commit <message>\n";
            return 1;
        }

        CommitCommand commit(".vcs");
        commit.execute(argv[2]);
    }
    else
    {
        std::cout << "Unknown command: " << command << '\n';
        return 1;
    }

    return 0;
}