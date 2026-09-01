#include "Commit.h"
#include <sstream>

Commit::Commit(const std::string& treeHash,
               const std::string& parentHash,
               const std::string& message)
    : treeHash(treeHash),
      parentHash(parentHash),
      message(message),
      timestamp(std::time(nullptr))
{
}

std::string Commit::serialize() const{

    // Serialize the commit metadata into a text format
    // that can be stored in the object database.
    std::ostringstream out;

    out << "tree " << treeHash <<'\n';
    
    // The initial commit has no parent.
    if(!parentHash.empty()){
        out << "parent " << parentHash <<'\n';
    }
    out << "time " << timestamp <<'\n';
    out << "message " << message <<'\n';

    // Return the serialized commit object.
    return out.str();
}


std::string Commit::getTreeHash(const std::string& data)
{
    std::istringstream in(data);
    std::string line;

    while (std::getline(in, line))
    {
        if (line.rfind("tree ", 0) == 0)
        {
            return line.substr(5);
        }
    }

    return {};
}