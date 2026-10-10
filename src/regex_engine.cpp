#include "regex_engine.h"

#include "elements/any_char_element.h"
#include "elements/char_class_element.h"
#include "elements/literal_element.h"
#include "enums.h"

void RegexEngine::Compile(const std::string& pattern) {
    tokens_.clear();
    const size_t len = pattern.length();
    size_t i = 0;

    while (i < len) {
        std::unique_ptr<RegexElement> element;

        if (pattern[i] == '.') {
            element = std::make_unique<AnyCharElement>();
            ++i;
        } else if (pattern[i] == '[') {
            const size_t close_pos = pattern.find(']', i + 1);
            if (close_pos == std::string::npos) {
                // можно выбросить исключение
                break;
            }

            std::vector<char> chars;
            std::vector<char> ranges;

            for (size_t j = i + 1; j < close_pos; ++j) {
                if (j + 2 < close_pos && pattern[j + 1] == '-') {
                    char start = pattern[j];
                    char end = pattern[j + 2];
                    if (start <= end) {
                        ranges.push_back(start);
                        ranges.push_back(end);
                    }
                    j += 2;
                } else {
                    chars.push_back(pattern[j]);
                }
            }

            element = std::make_unique<CharacterClassElement>(std::move(chars), std::move(ranges));
            i = close_pos + 1;
        } else {
            element = std::make_unique<LiteralElement>(pattern[i]);
            ++i;
        }

        QuantifierType quantifier = QuantifierType::None;
        if (i < len) {
            switch (pattern[i]) {
                case '*':
                    quantifier = QuantifierType::Star;
                    ++i;
                    break;
                case '+':
                    quantifier = QuantifierType::Plus;
                    ++i;
                    break;
                case '?':
                    quantifier = QuantifierType::Question;
                    ++i;
                    break;
                default:
                    quantifier = QuantifierType::None;
                    break;
            }
        }

        tokens_.emplace_back(std::move(element), quantifier);
    }
}

bool RegexEngine::MatchInternal_(const std::string& text, size_t token_idx, size_t str_idx) const {
    // 1. Все токены обработаны
    if (token_idx == tokens_.size()) {
        return str_idx == text.size();
    }

    // 2. Строка закончилась, но токены ещё остались
    if (str_idx == text.size()) {
        for (size_t i = token_idx; i < tokens_.size(); ++i) {
            const QuantifierType q = tokens_[i].Quantifier();
            if (q != QuantifierType::Star && q != QuantifierType::Question) {
                return false;
            }
        }
        return true;
    }

    const Token& token = tokens_[token_idx];
    const QuantifierType q = token.Quantifier();

    // 3. Без квантификатора (None)
    if (q == QuantifierType::None) {
        if (str_idx < text.size() && token.Matches(text[str_idx])) {
            return MatchInternal_(text, token_idx + 1, str_idx + 1);
        }
        return false;
    }

    // 4. Жадные квантификаторы (* и +) без возврата назад (possessive)
    if (q == QuantifierType::Star || q == QuantifierType::Plus) {
        size_t count = 0;
        while (str_idx + count < text.size() && token.Matches(text[str_idx + count])) {
            ++count;
        }

        if (q == QuantifierType::Plus && count == 0) {
            return false;
        }

        return MatchInternal_(text, token_idx + 1, str_idx + count);
    }

    // 5. Ленивый квантификатор (?)
    if (q == QuantifierType::Question) {
        // Шаг 1: сначала пробуем взять 0 символов
        if (MatchInternal_(text, token_idx + 1, str_idx)) {
            return true;
        }

        // Шаг 2: если с 0 символов не вышло, пробуем взять 1 символ
        if (str_idx < text.size() && token.Matches(text[str_idx])) {
            return MatchInternal_(text, token_idx + 1, str_idx + 1);
        }

        return false;
    }

    return false;
}

bool RegexEngine::Match(const std::string& text) { return MatchInternal_(text, 0, 0); }
