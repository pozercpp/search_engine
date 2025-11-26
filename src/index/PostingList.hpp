#pragma once

#include <vector>
#include <cstdint>
#include <document.hpp>

struct PostingList {
    DocId docId;
    uint32_t frequency;
    std::vector<uint32_t> positions;

    PostingList() = default;
    PostingList(DocId id, uint32_t freq, std::vector<uint32_t> pos = {})
        : docId(id), frequency(freq), positions(std::move(pos)) {}
};