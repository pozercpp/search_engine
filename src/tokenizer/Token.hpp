#pragma once

#include <string>
#include <cstdint>

struct Token {
    std::string text;
    size_t position;
    Token() = default;
    Token(std::string t, uint32_t pos) 
        : text(std::move(t)), position(pos) {}
};