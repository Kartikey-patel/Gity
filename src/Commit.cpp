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
    std::ostringstream out;

    out << "tree " << treeHash <<'\n';
    
    if(!parentHash.empty()){
        out << "parent " << parentHash <<'\n';
    }
    out << "time " << timestamp <<'\n';
    out << "message " << message <<'\n';

    return out.str();
}