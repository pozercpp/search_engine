#pragma once

#include "../index/InvertedIndex.hpp"
#include "BM25.hpp"
#include <cstdint>

using DocId = uint32_t;

class Scorer {
public:
    Scorer(const InvertedIndex& index);

    double score(const std::string& term, DocId docId) const;

    uint32_t getDocumentLength(DocId docId) const;

private:
    const InvertedIndex& index_;
    BM25 bm25_;

    mutable std::unordered_map<DocId, uint32_t> docLengths_;
    mutable bool lengthsCached_ = false;

    void cacheDocumentLengths() const;
};