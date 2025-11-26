#include "Scorer.hpp"
#include <iostream>

Scorer::Scorer(const InvertedIndex& index)
    : index_(index), bm25_(BM25Params(1.5, 0.75)) {
    cacheDocumentLengths();
}

void Scorer::cacheDocumentLengths() const {
    if (lengthsCached_) return;

    for (const auto& [term, postings] : index_.getAll()) {
        for (const auto& p : postings) {
            docLengths_[p.docId] += p.frequency;
        }
    }
    lengthsCached_ = true;

    std::cout << "Кэшировано длин документов: " << docLengths_.size() << "\n";
}

uint32_t Scorer::getDocumentLength(DocId docId) const {
    auto it = docLengths_.find(docId);
    return (it != docLengths_.end()) ? it->second : 0;
}

double Scorer::score(const std::string& term, DocId docId) const {
    const auto* postings = index_.getPostings(term);
    if (!postings) return 0.0;

    for (const auto& p : *postings) {
        if (p.docId == docId) {
            uint32_t totalDocs = index_.getDocumentCount();
            uint32_t docsWithTerm = static_cast<uint32_t>(postings->size());
            double avgDocLen = index_.getAvgDocumentLength();
            uint32_t docLen = getDocumentLength(docId);

            return bm25_.score(p.frequency, docLen, totalDocs, docsWithTerm, avgDocLen);
        }
    }
    return 0.0;
}