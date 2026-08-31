#pragma once
#include<filesystem>


/**
 * @class ObjectStore
 * @brief Manages storage and retrieval of VCS objects.
 *
 * Objects are stored inside the .gity/objects directory using
 * their SHA-1 hash as the identifier.
 *
 * Layout:
 * objects/
 * ├── ab/
 * │   └── cdef1234...
 * └── 9f/
 *     └── 8712abcd...
 *
 * The first two characters of the hash are used as the
 * directory name to avoid storing too many files in one folder.
 */

class ObjectStore{
    private:
        std::filesystem::path objectDirectory;

        bool isValidHash(const std::string& hash)const;
    public:

        /**
        * Creates an ObjectStore that operates on the given object directory.
        */
        ObjectStore(const std::filesystem::path& objectDirectory);   

        /**
         * Computes the filesystem path for an object hash.
         *
         * Example:
         * Hash:
         *   aaf4c61ddcc5e8a2...
         *
         * Path:
         *   objects/aa/f4c61ddcc5e8a2...
         */
        std::filesystem::path getObjectPath(const std::string& hash);

        /**
         * Checks whether an object with the given hash
         * already exists in the object database.
         */
        bool hasObject(const std::string& hash);  

        /**
         * Stores serialized object data on disk.
         *
         * The object is written only once because the hash uniquely
         * identifies its contents.
         */
        void storeObject(const std::string& hash, const std::string& data);  

        /**
         * Loads and returns the serialized contents of an object.
         *
         * This function is used when reconstructing blobs,
         * trees, or commits from the object database.
         */
        std::string loadObject(const std::string& hash);
};