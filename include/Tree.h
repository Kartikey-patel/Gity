#pragma once
#include<vector>

#include "Index.h"


class Tree{
private:
    std::vector<IndexEntry> entries;

public:
    explicit Tree(const std::vector<IndexEntry>& entries);

    std::string serialize() const;

};