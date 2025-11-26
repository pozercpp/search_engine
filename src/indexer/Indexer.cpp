#include <indexer.hpp>
#include <../tokenizer/tokenizer.hpp>
#include <../utils/FileSystem.hpp>
#include <../index/IndexSerializer.h>

#include <iostream>
#include <fstream>
#include <sstream>

void Indexer::buildIndexFromDirectory(const std::string& directoryPath) {
    auto files = FileSystem::getTextFiles(directoryPath);
    for (const auto& file : files) {
        addDocument(file);
    }
}

void Indexer::addDocument(const std::string& filePath) {
    auto file = readFileContent(filePath);
    if (file.empty()) {
        std::cerr << "Не удалось прочитать файл или он пустой: " << filePath << "\n";
        return;
    }
    Document doc(nextDocId, filePath, file);
    doc.extractTitleFromPath();
    indexSingleDocument(doc);
    ++nextDocId;
}

std::string Indexer::readFileContent(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) return {};
    std::ostream ss;
    ss << file.rdbuf();
    return ss.str();
}