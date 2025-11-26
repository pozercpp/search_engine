#include<invertedindex.hpp>
#include<algorithm>

void InvertedIndex::addPosting(const std::string& term, const PostingList& posting) { index[term].push_back(posting); }

const std::vector<PostingList>* InvertedIndex::getPostings(const std::string& term) const { 
    auto it = index.find(term);
    return (it != index.end() ? &it->second : nullptr);
}

void InvertedIndex::clear() {
    index.clear();
    docCount = 0;
    avgDocLength = 0.0;
}

const TermMap& InvertedIndex::getAll() const { return index; }

size_t InvertedIndex::size() const { return index.size(); }

size_t InvertedIndex::getDocumentCount() const { return docCount; }

void InvertedIndex::setDocumentCount(size_t n) { docCount = n; }

double InvertedIndex::getAvgDocumentLength() const { return avgDocLength; }

void InvertedIndex::setAvgDocumentLength(double len) { avgDocLength = len; }

size_t InvertedIndex::totalPostings() const {
    size_t total = 0;
    for (const auto& [term, list_] : index) {
        total += list_.size();
    }
    return total;
}