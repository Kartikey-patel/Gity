#pragma once
#include<filesystem>

class ObjectStore{
    private:
        std::filesystem::path objectDirectory;
    public:
        ObjectStore(const std::filesystem::path& objectDirectory);   

        bool hasObject(const std::string& hash);  

        std::filesystem::path getObjectPath(const std::string& hash);
        
        void storeObject(const std::string& hash, const std::string& data);  

        std::string loadObject(const std::string& hash);
};