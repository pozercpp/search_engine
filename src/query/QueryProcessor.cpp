#include "QueryProcessor.hpp"
#include "../tokenizer/Tokenizer.hpp"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <iostream>

QueryProcessor::QueryProcessor(const InvertedIndex& index)
    : index_(index), scorer_(index) {}

void QueryProcessor::setDocumentInfo(DocId id, const std::string& path, const std::string& title) {
    std::string t = title.empty() ? std::filesystem::path(path).filename().string() : title;
    docInfo_[id] = {path, t};
}

std::vector<std::string> QueryProcessor::tokenizeQuery(const std::string& query) const {
    Tokenizer tokenizer;
    std::string cleaned;
    for (char c : query) {
        if (std::isalnum(c) || c == '"' || c == '(' || c == ')' || std::isspace(c)) {
            cleaned += std::tolower(c);
        }
    }
    return tokenizer.tokenize(cleaned);
}

std::vector<SearchResult> QueryProcessor::search(const std::string& query, size_t topK) const {
    std::string q = query;
    for (char& c : q) c = std::tolower(c);

    std::vector<std::string> tokens = tokenizeQuery(q);
    if (tokens.empty()) return {};

    bool hasAND = false;
    bool hasOR = false;
    for (const auto& t : tokens) {
        if (t == "and") hasAND = true;
        if (t == "or") hasOR = true;
    }

    std::vector<DocId> candidates;

    if (hasAND && !hasOR) {
        // AND-запрос
        std::vector<std::vector<DocId>> postings;
        for (const auto& term : tokens) {
            if (term == "and" || term == "or") continue;
            auto* list = index_.getPostings(term);
            if (list) {
                std::vector<DocId> docs;
                for (const auto& p : *list) docs.push_back(p.docId);
                postings.push_back(std::move(docs));
            } else {
                return {};
            }
        }
        if (!postings.empty()) {
            candidates = postings[0];
            for (size_t i = 1; i < postings.size(); ++i) {
                candidates = intersect(candidates, postings[i]);
                if (candidates.empty()) break;
            }
        }
    } else {
        std::unordered_set<DocId> docSet;
        for (const auto& term : tokens) {
            if (term == "and" || term == "or") continue;
            auto* list = index_.getPostings(term);
            if (list) {
                for (const auto& p : *list) {
                    docSet.insert(p.docId);
                }
            }
        }
        candidates.assign(docSet.begin(), docSet.end());
    }

    std::vector<SearchResult> results;
    for (DocId docId : candidates) {
        double score = 0.0;
        for (const auto& term : tokens) {
            if (term == "and" || term == "or") continue;
            score += scorer_.score(term, docId);
        }
        auto it = docInfo_.find(docId);
        if (it != docInfo_.end()) {
            results.push_back({docId, score, it->second.path, it->second.title});
        }
    }

    std::sort(results.begin(), results.end());
    if (results.size() > topK) {
        results.resize(topK);
    }

    return results;
}

std::vector<DocId> QueryProcessor::intersect(const std::vector<DocId>& a,
                                             const std::vector<DocId>& b) const {
    std::vector<DocId> result;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(result));
    return result;
}

std::vector<DocId> QueryProcessor::unite(const std::vector<DocId>& a,
                                         const std::vector<DocId>& b) const {
    std::vector<DocId> result;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                   std::back_inserter(result));
    return result;
}