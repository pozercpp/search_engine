#include "IndexSerializer.hpp"
#include <fstream>
#include <nlohmann/json.hpp>  // https://github.com/nlohmann/json
#include <filesystem>

using json = nlohmann::json;

bool IndexSerializer::saveToJson(const InvertedIndex& index,
                                 const std::string& indexDir,
                                 const std::string& filename) {
    namespace fs = std::filesystem;
    fs::create_directories(indexDir);

    json j;

    // Метаданные
    j["metadata"]["doc_count"] = index.getDocumentCount();
    j["metadata"]["avg_doc_length"] = index.getAvgDocumentLength();
    j["metadata"]["total_terms"] = index.size();

    json indexJson = json::object();
    for (const auto& [term, postings] : index.getAll()) {
        json postingsArray = json::array();
        for (const auto& p : postings) {
            json posting;
            posting["doc"] = p.docId;
            posting["freq"] = p.frequency;
            posting["pos"] = p.positions;
            postingsArray.push_back(posting);
        }
        indexJson[term] = postingsArray;
    }
    j["index"] = indexJson;

    std::ofstream file(indexDir + "/" + filename);
    if (!file.is_open()) return false;

    file << j.dump(4);
    return true;
}

bool IndexSerializer::loadFromJson(InvertedIndex& index,
                                   const std::string& indexDir,
                                   const std::string& filename) {
    std::ifstream file(indexDir + "/" + filename);
    if (!file.is_open()) {
        return false;
    }

    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        return false;
    }

    index.clear();

    if (j.contains("metadata")) {
        auto& m = j["metadata"];
        index.setDocumentCount(m.value("doc_count", 0));
        index.setAvgDocumentLength(m.value("avg_doc_length", 0.0));
    }

    if (j.contains("index")) {
        for (auto& [term, postingsArray] : j["index"].items()) {
            for (const auto& p : postingsArray) {
                PostingList posting;
                posting.docId = p["doc"];
                posting.frequency = p["freq"];
                posting.positions = p["pos"].get<std::vector<uint32_t>>();
                index.addPosting(term, posting);
            }
        }
    }

    return true;
}