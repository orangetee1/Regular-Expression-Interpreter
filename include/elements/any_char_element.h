#pragma once

#include "regex_element.h"

// @brief Любой символ
class AnyCharElement : public RegexElement {
public:
    bool Matches(const char c);
};