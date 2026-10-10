#pragma once

#include <string>
#include <vector>

#include "token.h"

class RegexEngine {
public:
    void Compile(const std::string& pattern);
    bool Match(const std::string& text);

    std::vector<Token>& Tokens() { return tokens_; }

private:
    bool MatchInternal_(const std::string& text, size_t token_idx, size_t str_idx) const;

    std::vector<Token> tokens_;
};
