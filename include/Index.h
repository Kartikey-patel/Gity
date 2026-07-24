#pragma once

#include <vector>
#include <string>
#include <filesystem>

struct IndexEntry
{
    std::string hash;
    std::filesystem::path filePath;
};

class Index
{
private:
    std::filesystem::path indexPath;

    std::vector<IndexEntry> loadEntries();
    void saveEntries(const std::vector<IndexEntry>& entries);

public:
    std::vector<IndexEntry> getStagedEntries();

    explicit Index(const std::filesystem::path& indexPath);

    void addEntry(const std::string& hash, const std::filesystem::path& filePath);

};