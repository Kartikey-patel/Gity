#include "Tree.h"
#include <sstream>
#include<algorithm>

Tree::Tree(const std::vector<IndexEntry>& entries)
    : entries(entries)
{
    // Sort entries to ensure a deterministic tree representation.
    // The same set of files should always produce the same tree hash.
    std::sort(this->entries.begin(), this->entries.end(),
        [](const IndexEntry& lhs, const IndexEntry& rhs)
        {
            return lhs.filePath < rhs.filePath;
        });
}

std::string Tree::serialize() const{
    std::ostringstream out;
    
    // Serialize each staged file as:
    // <hash> <filepath>
    for(const auto entry : entries){
        out << entry.hash <<' ' << entry.filePath << '\n';
    }
    return out.str();
}