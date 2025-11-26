#include<vector>
#include<string>
#include<filesystem>

#include<FileSystem.hpp>

void FileSystem::getAllFiles(const std::string& dir, std::vector<std::string>& files) {
    for (const auto& file : std::filesystem::directory_iterator(dir)) {
        if (file.is_directory()) {
            getAllFiles(file.path().string(), files);
        } else if (file.is_regular_file()) {
            std::string ext = file.path().extension().string();
            auto Checktxt = [&](const std::string& file) -> bool {
                return (ext == ".txt" || ext == ".md" || ext == ".html" || ext == ".log" || ext == ".doc");
            };
            if (Checktxt(ext)) {
                files.push_back(file.path().string());
            }
        }
    }
}

std::vector<std::string> FileSystem::getTextFiles(const std::string& rootDir) {
    std::vector<std::string> files;
    getAllFiles(rootDir, files);
    return files;
}