#include "Tokenizer.hpp"
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iostream>

Tokenizer::Tokenizer() {
    loadStopWords("data/stop_words.txt");
}

std::vector<Token> Tokenizer::tokenize(const std::string& text) const {
    std::vector<Token> tokens;
    std::istringstream stream(text);
    std::string word;
    size_t position = 0;

    while (stream >> word) {
        std::string cleaned = removePunctuation(word);
        if (cleaned.empty()) continue;

        cleaned = toLower(cleaned);

        if (isStopWord(stopWords_, cleaned)) {
            continue;
        }

        cleaned = stem(cleaned);

        if (!cleaned.empty()) {
            tokens.emplace_back(cleaned, position++);
        }
    }

    return tokens;
}

std::string Tokenizer::toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string Tokenizer::removePunctuation(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalnum(c) || c == '-') {
            result += c;
        }
    }
    return result;
}

bool Tokenizer::isStopWord(const std::unordered_set<std::string>& stopWords,
                           const std::string& word) {
    return stopWords.count(word) > 0;
}

std::string Tokenizer::stem(const std::string& word) {
    return word;
}

void Tokenizer::addStopWord(const std::string& word) {
    stopWords_.insert(toLower(word));
}

bool Tokenizer::loadStopWords(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Не найден файл стоп-слов: " << filepath << "\n";
        return false;
    }

    std::string word;
    while (std::getline(file, word)) {
        if (!word.empty() && word[0] != '#') {
            word = toLower(word);
            if (!word.empty() && word.back() == '\r') word.pop_back();
            if (!word.empty()) {
                stopWords_.insert(word);
            }
        }
    }
    std::cout << "Загружено стоп-слов: " << stopWords_.size() << "\n";
    return true;
}