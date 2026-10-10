#include <iostream>
#include <string>

#include "regex_engine.h"

int main(int argc, char const* argv[]) {
    RegexEngine engine;

    std::string regex_line;
    std::getline(std::cin, regex_line);

    engine.Compile(regex_line);

    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::string result = engine.Match(line) ? "Match" : "Doesn't match";
        std::cout << result << std::endl;
    }

    return 0;
}
