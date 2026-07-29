#include"Hasher.h"

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

std::string Hasher::sha1(const std::string& data)
{
    // Compute the SHA-1 digest of the input string and
    // encode it as a hexadecimal string.

    std::string digest;

    CryptoPP::SHA1 hash;

    CryptoPP::StringSource(
        data,
        true,
        new CryptoPP::HashFilter(
            hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest),
                false
            )
        )
    );
      // Return the hexadecimal SHA-1 hash.
      
    return digest;
}