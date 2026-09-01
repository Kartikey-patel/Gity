#include "Index.h"

#include<fstream>
#include<algorithm>

Index::Index(const std::filesystem::path& indexPath)
    : indexPath(indexPath) {}


std::vector<IndexEntry> Index::loadEntries()const{
    std::vector<IndexEntry>entries;

    // Open the staging index.
    std::ifstream indexFile(indexPath);
    if(!indexFile.is_open()){
        return entries;
    } 

    // Read each staged entry from the index file.
    std::string hash;
    std::filesystem::path path;
    while (indexFile >> hash >> path){
        IndexEntry entry;
        entry.hash = hash;
        entry.filePath = path;
        entries.push_back(entry);
    }

    // Return all staged files.
    return entries;
}

void Index::saveEntries(const std::vector<IndexEntry>& entries)const{

    // Rewrite the index with the updated list of staged files.
    std::ofstream indexFile(indexPath);

    if(!indexFile.is_open()){
        return;
    }
    
    // Store one entry per line.
    for(const auto& entry : entries){
        indexFile << entry.hash << ' ' << entry.filePath << '\n';
    }

}

void Index::addEntry(const std::string& hash,const std::filesystem::path& filePath){
    // The staging area contains at most one entry per file.
    // Re-adding a file replaces its hash with the latest version
    // instead of creating duplicate entries.

    // Load the current staging area.
    auto entries = loadEntries();

    // Update the hash if the file is already staged.
    for(auto& entry : entries){
        if(entry.filePath == filePath){
            if(entry.hash != hash){
                entry.hash = hash;
            }
            saveEntries(entries);
            return;
        }
    }

    // Otherwise, stage it as a new file.
    IndexEntry entry;

    entry.hash = hash;
    entry.filePath = filePath;
    entries.push_back(entry);

    // Persist the updated staging area.
    saveEntries(entries);
}

std::vector<IndexEntry> Index::getEntries()const
{
    return loadEntries();
}

//clears the staging area after the commit
void Index::clear()const
{
    saveEntries({});
}


void Index::removeEntry(const std::filesystem::path& filePath)
{
    auto entries = loadEntries();

    entries.erase(std::remove_if(entries.begin(),entries.end(),[&](const IndexEntry& entry)
            {
                return entry.filePath == filePath;
            }),entries.end()
    );

    saveEntries(entries);
}