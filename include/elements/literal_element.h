#pragma once

#include "regex_element.h"

/// @brief Конкретный символ
class LiteralElement : public RegexElement {
public:
    LiteralElement() = delete;
    LiteralElement(const char ch) : ch_(ch) {}

    bool Matches(const char c);

private:
    char ch_;
};