#include "any_char_element.h"

#include <cctype>

bool AnyCharElement::Matches(const char c) { return std::isprint(static_cast<unsigned char>(c)); }