#pragma once
#include<iostream>
#include<filesystem>


/**
 * @class Hasher
 * @brief Generates SHA-1 hashes for VCS objects.
 *
 * This class provides a utility function to compute the SHA-1
 * hash of a string. The resulting hash is used as the unique
 * identifier for blobs, trees, and commits.
 */


class Hasher{
    public:
    /**
     * Computes the SHA-1 hash of the given data.
     *
     * @param data The input string to hash.
     * @return A 40-character hexadecimal SHA-1 hash.
     */
        std::string sha1(const std::string& data);
};