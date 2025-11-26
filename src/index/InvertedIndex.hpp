#pragma once

#include <postinglist.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class InvertedIndex {
public:
    using TermMap = std::unordered_map<std::string, std::vector<PostingList>>;
    
    void addPosting(const std::string& term, const PostingList& posting);

    const std::vector<PostingList>* getPostings(const std::string& term) const;

    const TermMap& getAll() const;

    void clear();
    size_t size() const;
    size_t totalPostings() const;

    size_t getDocumentCount() const;
    void setDocumentCount(size_t n);
    double getAvgDocumentLength() const;
    void setAvgDocumentLength(double len);

private:
    TermMap index;
    size_t docCount = 0;
    double avgDocLength = 0.0;
};