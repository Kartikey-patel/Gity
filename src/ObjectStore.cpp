#include "ObjectStore.h"
#include <fstream>
#include <cctype>
#include <stdexcept>

ObjectStore::ObjectStore(const std::filesystem::path& objectDirectory){   
    this->objectDirectory = objectDirectory;
}

std::filesystem::path ObjectStore::getObjectPath(const std::string& hash)const{  
// Git stores objects by splitting the SHA-1 hash.
// This prevents thousands of files from accumulating
// inside a single directory.

    if (!isValidHash(hash))
    {
        throw std::invalid_argument("Invalid SHA-1 hash");
    }

    std::string dir = hash.substr(0,2);
    std::string file = hash.substr(2);

    std::filesystem::path filePath = objectDirectory / dir / file;
    return filePath;
}

bool ObjectStore::hasObject(const std::string& hash)const{  
    
    // Identical objects share the same hash, so
    // there is no need to store duplicate copies.
    return std::filesystem::exists(getObjectPath(hash));
}


void ObjectStore::storeObject(const std::string& hash, const std::string& data){    
    if(hasObject(hash)){
        return;
    }
    
    // Compute the storage location based on the object's hash.
    auto objectPath = getObjectPath(hash);
    
    // Ensure the parent directory exists before writing the object.
    std::filesystem::create_directories(objectPath.parent_path());

    // Store the serialized object data.
    std::ofstream file(objectPath);
    file << data;
}

std::string ObjectStore::loadObject(const std::string& hash)const{

    // Open the object file corresponding to the given hash.
    std::ifstream file(getObjectPath(hash));

    if (!file.is_open())
    {
        return {};
    }

    // Read the complete contents of the object.
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Return the serialized object.
    return buffer.str();
}    


bool ObjectStore::isValidHash(const std::string& hash)const{
    if(hash.length() != 40){
        return false;
    }
    for(char c : hash){
        if(!std::isxdigit(static_cast<unsigned char>(c))){
            return false;
        }
    }
    return true;
}