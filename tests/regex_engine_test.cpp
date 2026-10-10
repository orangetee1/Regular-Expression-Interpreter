#include "regex_engine.h"

#include <gtest/gtest.h>

#include "enums.h"

// Тест на компиляцию пустой строки регулярного выражения
TEST(RegexEngineCompileTest, EmptyPattern) {
    RegexEngine engine;
    engine.Compile("");

    EXPECT_TRUE(engine.Tokens().empty());
}

// Тест на один литерал без квантификатора
TEST(RegexEngineCompileTest, SingleLiteral) {
    RegexEngine engine;
    engine.Compile("a");

    ASSERT_EQ(engine.Tokens().size(), 1);
    EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::None);
    EXPECT_TRUE(engine.Tokens()[0].Matches('a'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('b'));
}

// Тест на последовательность нескольких литералов
TEST(RegexEngineCompileTest, MultipleLiterals) {
    RegexEngine engine;
    engine.Compile("abc");

    ASSERT_EQ(engine.Tokens().size(), 3);

    EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::None);
    EXPECT_TRUE(engine.Tokens()[0].Matches('a'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('b'));

    EXPECT_EQ(engine.Tokens()[1].Quantifier(), QuantifierType::None);
    EXPECT_TRUE(engine.Tokens()[1].Matches('b'));
    EXPECT_FALSE(engine.Tokens()[1].Matches('c'));

    EXPECT_EQ(engine.Tokens()[2].Quantifier(), QuantifierType::None);
    EXPECT_TRUE(engine.Tokens()[2].Matches('c'));
    EXPECT_FALSE(engine.Tokens()[2].Matches('a'));
}

// Тест на метасимвол точки (любой символ)
TEST(RegexEngineCompileTest, AnyCharDot) {
    RegexEngine engine;
    engine.Compile(".");

    ASSERT_EQ(engine.Tokens().size(), 1);
    EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::None);

    EXPECT_TRUE(engine.Tokens()[0].Matches('a'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('Z'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('5'));
    EXPECT_TRUE(engine.Tokens()[0].Matches(' '));
    EXPECT_TRUE(engine.Tokens()[0].Matches('@'));
}

// Тест на группу символов с одиночными символами [abc]
TEST(RegexEngineCompileTest, CharacterClassSingleChars) {
    RegexEngine engine;
    engine.Compile("[abc]");

    ASSERT_EQ(engine.Tokens().size(), 1);
    EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::None);

    EXPECT_TRUE(engine.Tokens()[0].Matches('a'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('b'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('c'));

    EXPECT_FALSE(engine.Tokens()[0].Matches('d'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('A'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('1'));
}

// Тест на группу символов с диапазоном [a-d]
TEST(RegexEngineCompileTest, CharacterClassRange) {
    RegexEngine engine;
    engine.Compile("[a-d]");

    ASSERT_EQ(engine.Tokens().size(), 1);
    EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::None);

    EXPECT_TRUE(engine.Tokens()[0].Matches('a'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('b'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('c'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('d'));

    EXPECT_FALSE(engine.Tokens()[0].Matches('e'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('`'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('A'));
}

// Тест на группу со смешанными диапазонами и одиночными символами [a-c0-2_]
TEST(RegexEngineCompileTest, CharacterClassMixedRangesAndChars) {
    RegexEngine engine;
    engine.Compile("[a-c0-2_]");

    ASSERT_EQ(engine.Tokens().size(), 1);
    EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::None);

    // Диапазон a-c
    EXPECT_TRUE(engine.Tokens()[0].Matches('a'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('b'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('c'));

    // Диапазон 0-2
    EXPECT_TRUE(engine.Tokens()[0].Matches('0'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('1'));
    EXPECT_TRUE(engine.Tokens()[0].Matches('2'));

    // Одиночный символ '_'
    EXPECT_TRUE(engine.Tokens()[0].Matches('_'));

    // Несовпадающие
    EXPECT_FALSE(engine.Tokens()[0].Matches('d'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('3'));
    EXPECT_FALSE(engine.Tokens()[0].Matches(' '));
}

// Тест на литералы со всеми видами квантификаторов (*, +, ?)
TEST(RegexEngineCompileTest, LiteralQuantifiers) {
    // a* (ноль или более раз)
    {
        RegexEngine engine;
        engine.Compile("a*");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Star);
        EXPECT_TRUE(engine.Tokens()[0].Matches('a'));
    }

    // b+ (один или более раз)
    {
        RegexEngine engine;
        engine.Compile("b+");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Plus);
        EXPECT_TRUE(engine.Tokens()[0].Matches('b'));
    }

    // c? (ноль или один раз)
    {
        RegexEngine engine;
        engine.Compile("c?");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Question);
        EXPECT_TRUE(engine.Tokens()[0].Matches('c'));
    }

    // Последовательность a*b+c?
    {
        RegexEngine engine;
        engine.Compile("a*b+c?");
        ASSERT_EQ(engine.Tokens().size(), 3);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Star);
        EXPECT_EQ(engine.Tokens()[1].Quantifier(), QuantifierType::Plus);
        EXPECT_EQ(engine.Tokens()[2].Quantifier(), QuantifierType::Question);
    }
}

// Тест на квантификаторы для метасимвола точки (.*, .+, .?)
TEST(RegexEngineCompileTest, AnyCharQuantifiers) {
    {
        RegexEngine engine;
        engine.Compile(".*");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Star);
        EXPECT_TRUE(engine.Tokens()[0].Matches('z'));
    }
    {
        RegexEngine engine;
        engine.Compile(".+");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Plus);
        EXPECT_TRUE(engine.Tokens()[0].Matches('z'));
    }
    {
        RegexEngine engine;
        engine.Compile(".?");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Question);
        EXPECT_TRUE(engine.Tokens()[0].Matches('z'));
    }
}

// Тест на квантификаторы для группы символов ([a-z]*, [0-9]+, [abc]?)
TEST(RegexEngineCompileTest, CharacterClassQuantifiers) {
    {
        RegexEngine engine;
        engine.Compile("[a-z]*");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Star);
        EXPECT_TRUE(engine.Tokens()[0].Matches('m'));
        EXPECT_FALSE(engine.Tokens()[0].Matches('1'));
    }
    {
        RegexEngine engine;
        engine.Compile("[0-9]+");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Plus);
        EXPECT_TRUE(engine.Tokens()[0].Matches('7'));
        EXPECT_FALSE(engine.Tokens()[0].Matches('x'));
    }
    {
        RegexEngine engine;
        engine.Compile("[abc]?");
        ASSERT_EQ(engine.Tokens().size(), 1);
        EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Question);
        EXPECT_TRUE(engine.Tokens()[0].Matches('b'));
        EXPECT_FALSE(engine.Tokens()[0].Matches('d'));
    }
}

// Тест на составное регулярное выражение из условия задачи: "[a-z]+ [0-9]"
TEST(RegexEngineCompileTest, ComplexPatternExample) {
    RegexEngine engine;
    engine.Compile("[a-z]+ [0-9]");

    ASSERT_EQ(engine.Tokens().size(), 3);

    // 1. [a-z]+
    EXPECT_EQ(engine.Tokens()[0].Quantifier(), QuantifierType::Plus);
    EXPECT_TRUE(engine.Tokens()[0].Matches('h'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('0'));

    // 2. ' ' (пробел)
    EXPECT_EQ(engine.Tokens()[1].Quantifier(), QuantifierType::None);
    EXPECT_TRUE(engine.Tokens()[1].Matches(' '));
    EXPECT_FALSE(engine.Tokens()[1].Matches('a'));

    // 3. [0-9]
    EXPECT_EQ(engine.Tokens()[2].Quantifier(), QuantifierType::None);
    EXPECT_TRUE(engine.Tokens()[2].Matches('5'));
    EXPECT_FALSE(engine.Tokens()[2].Matches('z'));
}

// Тест на очистку предыдущего состояния при повторном вызове Compile()
TEST(RegexEngineCompileTest, RecompileClearsPreviousTokens) {
    RegexEngine engine;

    engine.Compile("abcdef");
    EXPECT_EQ(engine.Tokens().size(), 6);

    engine.Compile("x");
    ASSERT_EQ(engine.Tokens().size(), 1);
    EXPECT_TRUE(engine.Tokens()[0].Matches('x'));
    EXPECT_FALSE(engine.Tokens()[0].Matches('a'));
}

// Тест на незакрытую скобку '['
TEST(RegexEngineCompileTest, UnclosedBracketDoesNotCrash) {
    RegexEngine engine;
    // Незакрытая скобка не должна приводить к вылету или токену с nullptr
    engine.Compile("[a-z");

    EXPECT_TRUE(engine.Tokens().empty());
}

// ---------------------- Тесты для метода Match ----------------------

// Тест на пустой шаблон
TEST(RegexEngineMatchTest, EmptyPattern) {
    RegexEngine engine;
    engine.Compile("");

    EXPECT_TRUE(engine.Match(""));
    EXPECT_FALSE(engine.Match("a"));
    EXPECT_FALSE(engine.Match(" "));
}

// Тест на одиночный литерал
TEST(RegexEngineMatchTest, SingleLiteral) {
    RegexEngine engine;
    engine.Compile("a");

    EXPECT_TRUE(engine.Match("a"));
    EXPECT_FALSE(engine.Match(""));
    EXPECT_FALSE(engine.Match("b"));
    EXPECT_FALSE(engine.Match("aa"));
    EXPECT_FALSE(engine.Match(" a"));
}

// Тест на последовательность литералов
TEST(RegexEngineMatchTest, MultipleLiterals) {
    RegexEngine engine;
    engine.Compile("abc");

    EXPECT_TRUE(engine.Match("abc"));
    EXPECT_FALSE(engine.Match(""));
    EXPECT_FALSE(engine.Match("ab"));
    EXPECT_FALSE(engine.Match("abcd"));
    EXPECT_FALSE(engine.Match("abd"));
    EXPECT_FALSE(engine.Match("xabc"));
}

// Тест на метасимвол точки '.'
TEST(RegexEngineMatchTest, AnyCharDot) {
    RegexEngine engine;
    engine.Compile(".");

    EXPECT_TRUE(engine.Match("a"));
    EXPECT_TRUE(engine.Match("Z"));
    EXPECT_TRUE(engine.Match("5"));
    EXPECT_TRUE(engine.Match(" "));
    EXPECT_TRUE(engine.Match("@"));

    EXPECT_FALSE(engine.Match(""));
    EXPECT_FALSE(engine.Match("ab"));

    engine.Compile(".b.");
    EXPECT_TRUE(engine.Match("abc"));
    EXPECT_TRUE(engine.Match("1b2"));
    EXPECT_FALSE(engine.Match("ac"));
}

// Тест на класс символов '[...]'
TEST(RegexEngineMatchTest, CharacterClass) {
    RegexEngine engine;
    engine.Compile("[abc]");
    EXPECT_TRUE(engine.Match("a"));
    EXPECT_TRUE(engine.Match("b"));
    EXPECT_TRUE(engine.Match("c"));
    EXPECT_FALSE(engine.Match("d"));
    EXPECT_FALSE(engine.Match(""));
    EXPECT_FALSE(engine.Match("ab"));

    engine.Compile("[a-d]");
    EXPECT_TRUE(engine.Match("a"));
    EXPECT_TRUE(engine.Match("b"));
    EXPECT_TRUE(engine.Match("c"));
    EXPECT_TRUE(engine.Match("d"));
    EXPECT_FALSE(engine.Match("e"));

    engine.Compile("[a-c0-2_]");
    EXPECT_TRUE(engine.Match("b"));
    EXPECT_TRUE(engine.Match("1"));
    EXPECT_TRUE(engine.Match("_"));
    EXPECT_FALSE(engine.Match("d"));
    EXPECT_FALSE(engine.Match("3"));
}

// Тест на жадный квантификатор '*' без отката назад (possessive)
TEST(RegexEngineMatchTest, GreedyStarPossessive) {
    RegexEngine engine;
    engine.Compile("a*");

    EXPECT_TRUE(engine.Match(""));
    EXPECT_TRUE(engine.Match("a"));
    EXPECT_TRUE(engine.Match("aa"));
    EXPECT_TRUE(engine.Match("aaaaa"));
    EXPECT_FALSE(engine.Match("b"));
    EXPECT_FALSE(engine.Match("ab"));

    engine.Compile("a*b");
    EXPECT_TRUE(engine.Match("b"));
    EXPECT_TRUE(engine.Match("ab"));
    EXPECT_TRUE(engine.Match("aaab"));
    EXPECT_FALSE(engine.Match("a"));
    EXPECT_FALSE(engine.Match("ba"));

    // Требование из ТЗ: строка "aa" НЕ соответствует "a*a",
    // так как "a*" забирает все символы 'a', и для "a" ничего не остаётся
    engine.Compile("a*a");
    EXPECT_FALSE(engine.Match("aa"));
    EXPECT_FALSE(engine.Match("a"));
    EXPECT_FALSE(engine.Match(""));
    EXPECT_FALSE(engine.Match("aaa"));
}

// Тест на жадный квантификатор '+' без отката назад (possessive)
TEST(RegexEngineMatchTest, GreedyPlusPossessive) {
    RegexEngine engine;
    engine.Compile("a+");

    EXPECT_FALSE(engine.Match(""));
    EXPECT_TRUE(engine.Match("a"));
    EXPECT_TRUE(engine.Match("aa"));
    EXPECT_TRUE(engine.Match("aaaa"));
    EXPECT_FALSE(engine.Match("b"));

    engine.Compile("a+b");
    EXPECT_FALSE(engine.Match("b"));
    EXPECT_TRUE(engine.Match("ab"));
    EXPECT_TRUE(engine.Match("aaab"));

    // Требование из ТЗ: строка "aa" НЕ соответствует "a+a",
    // так как "a+" забирает все символы 'a', и для "a" ничего не остаётся
    engine.Compile("a+a");
    EXPECT_FALSE(engine.Match("aa"));
    EXPECT_FALSE(engine.Match("a"));
    EXPECT_FALSE(engine.Match("aaa"));
}

// Тест на ленивый квантификатор '?'
TEST(RegexEngineMatchTest, LazyQuestion) {
    RegexEngine engine;
    engine.Compile("a?");

    EXPECT_TRUE(engine.Match(""));
    EXPECT_TRUE(engine.Match("a"));
    EXPECT_FALSE(engine.Match("b"));
    EXPECT_FALSE(engine.Match("aa"));

    // Требование из ТЗ: строка "a" сопоставляется с "a?a",
    // так как "a?" сопоставляется с пустой строкой "", а "a" сопоставляется со строкой "a"
    engine.Compile("a?a");
    EXPECT_TRUE(engine.Match("a"));
    EXPECT_FALSE(engine.Match(""));

    engine.Compile("a?b");
    EXPECT_TRUE(engine.Match("b"));
    EXPECT_TRUE(engine.Match("ab"));
    EXPECT_FALSE(engine.Match("a"));
    EXPECT_FALSE(engine.Match("aab"));
}

// Тест на составной шаблон из условия ТЗ: "[a-z]+ [0-9]"
TEST(RegexEngineMatchTest, ComplexPatternExample) {
    RegexEngine engine;
    engine.Compile("[a-z]+ [0-9]");

    EXPECT_TRUE(engine.Match("hello 5"));
    EXPECT_TRUE(engine.Match("cat 0"));
    EXPECT_TRUE(engine.Match("a 9"));

    EXPECT_FALSE(engine.Match("hello  5"));  // два пробела
    EXPECT_FALSE(engine.Match("hello 55"));  // две цифры
    EXPECT_FALSE(engine.Match("123 4"));     // цифры вместо букв
    EXPECT_FALSE(engine.Match("hello"));     // нет пробела и цифры
    EXPECT_FALSE(engine.Match(""));
}

// Тест на оставшиеся токены в конце шаблона, допускающие 0 символов
TEST(RegexEngineMatchTest, TrailingZeroQuantifiers) {
    RegexEngine engine;
    engine.Compile("a*b?");

    EXPECT_TRUE(engine.Match(""));
    EXPECT_TRUE(engine.Match("a"));
    EXPECT_TRUE(engine.Match("aaa"));
    EXPECT_TRUE(engine.Match("b"));
    EXPECT_TRUE(engine.Match("ab"));
    EXPECT_TRUE(engine.Match("aaab"));
    EXPECT_FALSE(engine.Match("c"));

    engine.Compile("a*b+");
    EXPECT_FALSE(engine.Match(""));
    EXPECT_FALSE(engine.Match("a"));
    EXPECT_TRUE(engine.Match("b"));
    EXPECT_TRUE(engine.Match("ab"));
}
