#pragma once

#include <string>
#include <vector>

#include "token.h"

class RegexEngine {
public:
    void Compile(const std::string &pattern);
    bool Match(const std::string &text);

private:
    std::vector<Token> tokens_;
};