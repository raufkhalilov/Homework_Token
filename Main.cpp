#include <iostream>
#include <vector>
#include <unordered_map>
#include <variant>
#include <cctype>
#include <string>

struct OpeningBracket {};
struct ClosingBracket {};
struct Number {
    int value;
};
struct UnknownToken {
    std::string value;
};
struct Plus {};
struct Minus {};
struct Multiply {};
struct Divide {};
struct Modulo {};
struct MaxToken {};
struct SqrToken {};
struct Comma {}; // Добавляем Comma

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, Plus, Minus, Multiply, Divide, Modulo, MaxToken, SqrToken, Comma>; // Добавляем Comma

const std::unordered_map<char, Token> kSymbol2Token{
    {'+', Plus{}},
    {'-', Minus{}},
    {'*', Multiply{}},
    {'/', Divide{}},
    {'%', Modulo{}},
    {'(', OpeningBracket{}},
    {')', ClosingBracket{}},
    {',', Comma{}} // Добавляем Comma
};

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    while (pos < input.size() && std::isdigit(input[pos])) {
        value = value * 10 + ToDigit(input[pos]);
        ++pos;
    }
    return Number{ value };
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string name;
    while (pos < input.size() && std::isalpha(input[pos])) {
        name += input[pos++];
    }
    if (name == "max") {
        return MaxToken{};
    }
    else if (name == "sqr") {
        return SqrToken{};
    }
    else {
        return UnknownToken{ name };
    }
}

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t pos = 0;
    while (pos < input.size()) {
        char symbol = input[pos];
        if (std::isspace(symbol)) {
            ++pos;
        }
        else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        }
        else if (std::isalpha(symbol)) {
            tokens.emplace_back(ParseName(input, pos));
        }
        else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
            ++pos;
        }
        else {
            tokens.emplace_back(UnknownToken{ std::string(1, symbol) });
            ++pos;
        }
    }
    return tokens;
}

int main() {
    std::string expression = "max(1, 2) + sqr(3) - 4 / 5";
    auto tokens = Tokenize(expression);
    for (const auto& token : tokens) {
        std::visit([](const auto& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Number>) {
                std::cout << "Number: " << arg.value << std::endl;
            }
            else if constexpr (std::is_same_v<T, Plus>) {
                std::cout << "Operator: +" << std::endl;
            }
            else if constexpr (std::is_same_v<T, Minus>) {
                std::cout << "Operator: -" << std::endl;
            }
            else if constexpr (std::is_same_v<T, Multiply>) {
                std::cout << "Operator: *" << std::endl;
            }
            else if constexpr (std::is_same_v<T, Divide>) {
                std::cout << "Operator: /" << std::endl;
            }
            else if constexpr (std::is_same_v<T, Modulo>) {
                std::cout << "Operator: %" << std::endl;
            }
            else if constexpr (std::is_same_v<T, MaxToken>) {
                std::cout << "Function: max" << std::endl;
            }
            else if constexpr (std::is_same_v<T, SqrToken>) {
                std::cout << "Function: sqr" << std::endl;
            }
            else if constexpr (std::is_same_v<T, OpeningBracket>) {
                std::cout << "Opening Bracket: (" << std::endl;
            }
            else if constexpr (std::is_same_v<T, ClosingBracket>) {
                std::cout << "Closing Bracket: )" << std::endl;
            }
            else if constexpr (std::is_same_v<T, Comma>) { // Добавляем обработку Comma
                std::cout << "Comma: ," << std::endl;
            }
            else if constexpr (std::is_same_v<T, UnknownToken>) {
                std::cout << "Unknown: " << arg.value << std::endl;
            }
            }, token);
    }
    return 0;
}
