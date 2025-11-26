#pragma once

#include<vector>
#include<string>
#include <filesystem>

struct FileSystem {
public:
    std::vector<std::string> getTextFiles(const std::string& rootDir);
private:
    void getAllFiles(std::string& dir, std::vector<std::string>& files);
};