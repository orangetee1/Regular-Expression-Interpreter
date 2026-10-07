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
    std::vector<Token> tokens_;
};