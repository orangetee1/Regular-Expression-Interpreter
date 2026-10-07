#include "char_class_element.h"

bool CharacterClassElement::Matches(const char c) {
    for (char ch : chars_) {
        if (c == ch) {
            return true;
        }
    }
    for (size_t i = 0; i + 1 < ranges_.size(); i += 2) {
        if (c >= ranges_[i] && c <= ranges_[i + 1]) {
            return true;
        }
    }
    return false;
}