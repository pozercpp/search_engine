#pragma once

#include "../index/InvertedIndex.hpp"
#include "../ranking/Scorer.hpp"
#include "SearchResult.hpp"
#include <vector>
#include <string>
#include <unordered_map>

class QueryProcessor {
public:
    explicit QueryProcessor(const InvertedIndex& index);

    std::vector<SearchResult> search(const std::string& query, size_t topK = 10) const;

    void setDocumentInfo(DocId id, const std::string& path, const std::string& title = "");

private:
    const InvertedIndex& index_;
    Scorer scorer_;
    struct DocInfo {
        std::string path;
        std::string title;
    };
    std::unordered_map<DocId, DocInfo> docInfo_;

    std::vector<std::string> tokenizeQuery(const std::string& query) const;
    std::vector<DocId> executeQuery(const std::vector<std::string>& tokens) const;
    std::vector<DocId> intersect(const std::vector<DocId>& a, const std::vector<DocId>& b) const;
    std::vector<DocId> unite(const std::vector<DocId>& a, const std::vector<DocId>& b) const;
};