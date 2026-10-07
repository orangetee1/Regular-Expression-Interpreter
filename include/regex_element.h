#pragma once

class RegexElement {
public:
    virtual bool Matches(const char c) = 0;
    virtual ~RegexElement() = default;
};