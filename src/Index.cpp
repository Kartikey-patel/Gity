#include "Index.h"
#include<fstream>


Index::Index(const std::filesystem::path& indexPath)
    : indexPath(indexPath) {}

std::vector<IndexEntry> Index::loadEntries(){
    std::vector<IndexEntry>entries;

    std::ifstream indexFile(indexPath);
    if(!indexFile.is_open()){
        return entries;
    } 

    std::string hash;
    std::filesystem::path path;
    while (indexFile >> hash >> path){
        IndexEntry entry;
        entry.hash = hash;
        entry.filePath = path;
        entries.push_back(entry);
    }
    return entries;
}

void Index::saveEntries(const std::vector<IndexEntry>& entries){
    std::ofstream indexFile(indexPath);
    
    if(!indexFile.is_open()){
        return;
    }

    for(const auto& entry : entries){
        indexFile << entry.hash << ' ' << entry.filePath << '\n';
    }

}

void Index::addEntry(const std::string& hash,const std::filesystem::path& filePath){
    auto entries = loadEntries();

    for(auto& entry : entries){
        if(entry.filePath == filePath){
            if(entry.hash != hash){
                entry.hash = hash;
            }
            saveEntries(entries);
            return;
        }
    }
    IndexEntry entry;

    entry.hash = hash;
    entry.filePath = filePath;
    entries.push_back(entry);

    saveEntries(entries);
}

std::vector<IndexEntry> Index::getStagedEntries(){
    return loadEntries();
}