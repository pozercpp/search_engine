#pragma once

#include "InvertedIndex.hpp"
#include <string>

class IndexSerializer {
public:
    static bool saveToJson(const InvertedIndex& index,
                           const std::string& indexDir,
                           const std::string& filename = "inverted_index.json");

    static bool loadFromJson(InvertedIndex& index,
                             const std::string& indexDir,
                             const std::string& filename = "inverted_index.json");
};