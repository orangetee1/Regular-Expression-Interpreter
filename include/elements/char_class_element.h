#pragma once

#include <vector>

#include "regex_element.h"

// @brief Группа символов. Хранит список одиночных символов и диапазонов
class CharacterClassElement : public RegexElement {
public:
    CharacterClassElement() = default;
    CharacterClassElement(std::vector<char> chars, std::vector<char> ranges)
        : ranges_(std::move(ranges)), chars_(std::move(chars)) {}

    bool Matches(const char c) override;

private:
    std::vector<char> ranges_;
    std::vector<char> chars_;
};