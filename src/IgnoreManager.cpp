#include "IgnoreManager.h"

#include<fstream>

IgnoreManager::IgnoreManager(const std::filesystem::path& projectRoot)
    : projectRoot(projectRoot)
{
    loadIgnoreFiles();
}


void IgnoreManager::loadIgnoreFiles(){
    auto ignoreFilePath = projectRoot / ".gityignore";

    std::ifstream file(ignoreFilePath);

    if(!file.is_open()){
        return;
    }

    std::string line;

    while(std::getline(file,line)){
        if(line.empty() || line[0] == '#'){
            continue;
        }
        patterns.push_back(line);
    }
}

bool IgnoreManager::matchesPattern(const std::filesystem::path& path,const std::string& pattern)const{

    auto wildcardPosition = pattern.find('*');
    if(wildcardPosition != std::string::npos){
        std::string suffix = pattern.substr(wildcardPosition+1);
        
        std::string filename = path.filename().string();

        if (filename.length() < suffix.length())
        {
            return false;
        }

        return filename.compare(
                   filename.length() - suffix.length(),
                   suffix.length(),
                   suffix
               ) == 0;
        }

    if(pattern.empty()){
        return false;
    }
    if(pattern.back() == '/'){
        std::string directoryPattern = pattern.substr(0, pattern.length()-1);

        for(const auto& component : path){
            if(component == directoryPattern){
                return true;
            }
        }
        return false;
    }

    return path == std::filesystem::path(pattern);
}


bool IgnoreManager::isIgnored(const std::filesystem::path& path)const{
    for(const auto pattern : patterns){
        if(matchesPattern(path,pattern)){
            return true;
        }
    }
    return false;
}