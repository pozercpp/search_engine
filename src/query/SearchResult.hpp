#pragma once

#include <string>
#include <cstdint>

using DocId = uint32_t;

struct SearchResult {
    DocId docId;
    double score;
    std::string path;
    std::string title;
    bool operator<(const SearchResult& other) const {
        return score > other.score;
    }
};