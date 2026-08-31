#pragma once

#include<filesystem>
#include<vector>
#include<string>

class IgnoreManager{
    public:
        explicit IgnoreManager(const std::filesystem::path& projectRoot);

        bool isIgnored(const std::filesystem::path& path)const;

    private:
        std::filesystem::path projectRoot;
        std::vector<std::string> patterns;

        bool matchesPattern(const std::filesystem::path& path,const std::string& pattern) const;

        void loadIgnoreFiles();
};
