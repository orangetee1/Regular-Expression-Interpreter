#pragma once

#include <memory>

#include "enums.h"
#include "regex_element.h"

class Token {
public:
    Token(std::unique_ptr<RegexElement> element, QuantifierType quantifier)
        : element_(std::move(element)), quantifier_(quantifier) {}

    bool Matches(char c) { return element_.get()->Matches(c); }
    QuantifierType Quantifier() const { return quantifier_; }

private:
    std::unique_ptr<RegexElement> element_;
    QuantifierType quantifier_;
};