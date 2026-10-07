#include "char_class_element.h"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "regex_element.h"

// Тест на класс по умолчанию (пустой)
TEST(CharacterClassElementTest, DefaultConstructedIsEmpty) {
    CharacterClassElement element;

    EXPECT_FALSE(element.Matches('a'));
    EXPECT_FALSE(element.Matches('Z'));
    EXPECT_FALSE(element.Matches('0'));
    EXPECT_FALSE(element.Matches(' '));
    EXPECT_FALSE(element.Matches('\0'));
}

// Тест на явно заданный пустой класс
TEST(CharacterClassElementTest, ExplicitEmptyClass) {
    CharacterClassElement element({}, {});

    EXPECT_FALSE(element.Matches('a'));
    EXPECT_FALSE(element.Matches('1'));
    EXPECT_FALSE(element.Matches('!'));
}

// Тест только на список одиночных символов (без диапазонов)
TEST(CharacterClassElementTest, SingleCharactersMatching) {
    CharacterClassElement element({'a', 'x', '7', '_'}, {});

    // Символы из списка должны совпадать
    EXPECT_TRUE(element.Matches('a'));
    EXPECT_TRUE(element.Matches('x'));
    EXPECT_TRUE(element.Matches('7'));
    EXPECT_TRUE(element.Matches('_'));

    // Символы не из списка не должны совпадать
    EXPECT_FALSE(element.Matches('b'));
    EXPECT_FALSE(element.Matches('A'));
    EXPECT_FALSE(element.Matches('0'));
    EXPECT_FALSE(element.Matches(' '));
    EXPECT_FALSE(element.Matches('-'));
}

// Тест на один диапазон символов (например 'a' - 'e')
TEST(CharacterClassElementTest, SingleRangeMatching) {
    // Диапазон 'a' - 'e' (содержит 'a', 'b', 'c', 'd', 'e')
    CharacterClassElement element({}, {'a', 'e'});

    // Границы диапазона
    EXPECT_TRUE(element.Matches('a'));
    EXPECT_TRUE(element.Matches('e'));

    // Символы внутри диапазона
    EXPECT_TRUE(element.Matches('b'));
    EXPECT_TRUE(element.Matches('c'));
    EXPECT_TRUE(element.Matches('d'));

    // Соседние символы вне диапазона
    EXPECT_FALSE(element.Matches('`'));  // ASCII 96 (перед 'a')
    EXPECT_FALSE(element.Matches('f'));  // ASCII 102 (после 'e')
    EXPECT_FALSE(element.Matches('z'));
    EXPECT_FALSE(element.Matches('A'));
    EXPECT_FALSE(element.Matches('1'));
}

// Тест на вырожденный диапазон из одного символа ('a' - 'a')
TEST(CharacterClassElementTest, SingleCharRange) {
    CharacterClassElement element({}, {'a', 'a'});

    EXPECT_TRUE(element.Matches('a'));
    EXPECT_FALSE(element.Matches('b'));
    EXPECT_FALSE(element.Matches('`'));
}

// Тест на несколько диапазонов (например цифры '0'-'9' и заглавные 'A'-'Z')
TEST(CharacterClassElementTest, MultipleRangesMatching) {
    CharacterClassElement element({}, {'0', '9', 'A', 'Z'});

    // Цифры
    EXPECT_TRUE(element.Matches('0'));
    EXPECT_TRUE(element.Matches('5'));
    EXPECT_TRUE(element.Matches('9'));

    // Заглавные буквы
    EXPECT_TRUE(element.Matches('A'));
    EXPECT_TRUE(element.Matches('M'));
    EXPECT_TRUE(element.Matches('Z'));

    // Строчные буквы не должны совпадать
    EXPECT_FALSE(element.Matches('a'));
    EXPECT_FALSE(element.Matches('z'));

    // Граничные спецсимволы по таблице ASCII
    EXPECT_FALSE(element.Matches('/'));  // ASCII 47 (перед '0')
    EXPECT_FALSE(element.Matches(':'));  // ASCII 58 (после '9')
    EXPECT_FALSE(element.Matches('@'));  // ASCII 64 (перед 'A')
    EXPECT_FALSE(element.Matches('['));  // ASCII 91 (после 'Z')
}

// Тест на смешанный класс: одиночные символы и диапазоны
TEST(CharacterClassElementTest, MixedCharsAndRanges) {
    // Диапазоны: 'a'-'c', '0'-'2'; одиночные символы: '_', ' '
    CharacterClassElement element({'_', ' '}, {'a', 'c', '0', '2'});

    // Проверка диапазонов
    EXPECT_TRUE(element.Matches('a'));
    EXPECT_TRUE(element.Matches('b'));
    EXPECT_TRUE(element.Matches('c'));
    EXPECT_TRUE(element.Matches('0'));
    EXPECT_TRUE(element.Matches('1'));
    EXPECT_TRUE(element.Matches('2'));

    // Проверка одиночных символов
    EXPECT_TRUE(element.Matches('_'));
    EXPECT_TRUE(element.Matches(' '));

    // Проверка несовпадающих символов
    EXPECT_FALSE(element.Matches('d'));
    EXPECT_FALSE(element.Matches('3'));
    EXPECT_FALSE(element.Matches('A'));
    EXPECT_FALSE(element.Matches('-'));
}

// Тест на пробельные и спецсимволы
TEST(CharacterClassElementTest, WhitespaceAndSpecialChars) {
    CharacterClassElement element({' ', '\t', '\n', '\r'}, {});

    EXPECT_TRUE(element.Matches(' '));
    EXPECT_TRUE(element.Matches('\t'));
    EXPECT_TRUE(element.Matches('\n'));
    EXPECT_TRUE(element.Matches('\r'));

    EXPECT_FALSE(element.Matches('a'));
    EXPECT_FALSE(element.Matches('0'));
}

// Тест на некорректный / перевернутый диапазон ('z' - 'a')
TEST(CharacterClassElementTest, InvertedRangeDoesNotMatch) {
    // При 'z' > 'a' условие c >= 'z' && c <= 'a' невозможно
    CharacterClassElement element({}, {'z', 'a'});

    EXPECT_FALSE(element.Matches('a'));
    EXPECT_FALSE(element.Matches('m'));
    EXPECT_FALSE(element.Matches('z'));
}

// Тест на полиморфный вызов через базовый класс RegexElement
TEST(CharacterClassElementTest, PolymorphismViaRegexElement) {
    std::unique_ptr<RegexElement> element = std::make_unique<CharacterClassElement>(
        std::vector<char>{'x'}, std::vector<char>{'0', '9'});

    EXPECT_TRUE(element->Matches('x'));
    EXPECT_TRUE(element->Matches('0'));
    EXPECT_TRUE(element->Matches('5'));
    EXPECT_TRUE(element->Matches('9'));

    EXPECT_FALSE(element->Matches('y'));
    EXPECT_FALSE(element->Matches('a'));
}
