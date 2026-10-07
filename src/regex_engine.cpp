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
                // Если скобка не закрыта, можно прервать разбор или выбросить исключение
                break;
            }

            std::vector<char> chars;
            std::vector<char> ranges;

            for (size_t j = i + 1; j < close_pos; ++j) {
                // Если между символами стоит '-' (например a-d)
                if (j + 2 < close_pos && pattern[j + 1] == '-') {
                    char start = pattern[j];
                    char end = pattern[j + 2];
                    if (start <= end) {
                        ranges.push_back(start);
                        ranges.push_back(end);
                    }
                    j += 2;  // Пропускаем дефис и конечный символ диапазона
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

bool RegexEngine::Match(const std::string& /*text*/) { return false; }