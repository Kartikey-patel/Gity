#include "Tree.h"
#include <sstream>
#include<algorithm>


Tree::Tree(const std::vector<IndexEntry>& entries)
    : entries(entries)
{
    std::sort(this->entries.begin(), this->entries.end(),
        [](const IndexEntry& lhs, const IndexEntry& rhs)
        {
            return lhs.filePath < rhs.filePath;
        });
}

std::string Tree::serialize() const{
    std::ostringstream out;

    for(const auto entry : entries){
        out << entry.hash <<' ' << entry.filePath << '\n';
    }
    return out.str();
}