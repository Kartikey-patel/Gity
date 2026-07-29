#include "ObjectStore.h"
#include <fstream>

ObjectStore::ObjectStore(const std::filesystem::path& objectDirectory){   //constructor
    this->objectDirectory = objectDirectory;
}

std::filesystem::path ObjectStore::getObjectPath(const std::string& hash){   //  return the path 
    std::string dir = hash.substr(0,2);
    std::string file = hash.substr(2);

    std::filesystem::path filePath = objectDirectory / dir / file;
    return filePath;
}

bool ObjectStore::hasObject(const std::string& hash){   // check it the hash is present in the object dir or not

    return std::filesystem::exists(getObjectPath(hash));
}

void ObjectStore::storeObject(const std::string& hash, const std::string& data){    // create the directory and write the data to the file
    if(hasObject(hash)){
        return;
    }
    
    auto objectPath = getObjectPath(hash);
    
    std::filesystem::create_directories(objectPath.parent_path());

    std::ofstream file(objectPath);

    file << data;
}

std::string ObjectStore::loadObject(const std::string& hash){
    std::ifstream file(getObjectPath(hash));

    if (!file.is_open())
    {
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}    