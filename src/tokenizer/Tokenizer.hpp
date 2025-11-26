#pragma once

#include <string>
#include <vector>
#include <unordered_set>

struct Token {
    std::string text;
    size_t position;

    Token(std::string t, size_t pos) : text(std::move(t)), position(pos) {}
};

class Tokenizer {
public:
    Tokenizer();

    std::vector<Token> tokenize(const std::string& text) const;

    void addStopWord(const std::string& word);

    bool loadStopWords(const std::string& filepath);

private:
    std::unordered_set<std::string> stopWords_;

    static std::string toLower(std::string s);
    static std::string removePunctuation(const std::string& word);
    static bool isStopWord(const std::unordered_set<std::string>& stopWords, const std::string& word);
    
    static std::string stem(const std::string& word);
};