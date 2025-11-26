#pragma once

#include <string>
#include <cstdint>

using DocId = uint32_t;

struct Document {
    DocId id;
    std::string path;
    std::string title;
    std::string content;

    Document() = default;
    Document(DocId id, const std::string& path, const std::string& content = "")
        : id(id), path(path), content(content) {}
    
    void extractTitleFromPath() {
        size_t pos = path.find_last_of("/\\");
        title = (pos == std::string::npos) ? path : path.substr(pos + 1);
    }
};