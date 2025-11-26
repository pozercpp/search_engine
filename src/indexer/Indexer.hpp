#pragma once

#include <document.hpp>
#include<../index/invertedindex.hpp>
#include <string>
#include <vector>

class Indexer {
public:
    Indexer();
    ~Indexer() = default;

    void buildIndexFromDirectory(const std::string& directoryPath);

    void addDocument(const std::string& filePath);

    bool saveIndex(const std::string& indexDir) const;

    bool loadIndex(const std::string& indexDir);

    const InvertedIndex& getInvertedIndex() const { return invertedIndex; }

    size_t getTotalDocuments() const { return nextDocId - 1; }
    size_t getTotalTerms() const { return invertedIndex.size(); }

private:
    InvertedIndex invertedIndex;
    DocId nextDocId; 

    std::string readFileContent(const std::string& filePath) const;
    void indexSingleDocument(const Document& doc);
};