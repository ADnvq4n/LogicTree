#include "NaturalLanguageParser.h"

#include <cctype>
#include <stdexcept>
#include <utility>

NaturalLanguageParser::NaturalLanguageParser(
    KnowledgeBase& knowledgeBase
) : knowledgeBase(knowledgeBase) {
}

std::string NaturalLanguageParser::trim(
    const std::string& text
) const {
    std::size_t first = text.find_first_not_of(" \t\n\r");

    if (first == std::string::npos) {
        return "";
    }

    std::size_t last = text.find_last_not_of(" \t\n\r");
    return text.substr(first, last - first + 1);
}

std::string NaturalLanguageParser::normalize(
    const std::string& text
) const {
    std::string result;
    bool previousSpace = false;

    for (unsigned char character : text) {
        if (std::isspace(character)) {
            if (!result.empty() && !previousSpace) {
                result += ' ';
            }
            previousSpace = true;
        } else {
            result += static_cast<char>(std::tolower(character));
            previousSpace = false;
        }
    }

    return trim(result);
}

std::size_t NaturalLanguageParser::findTopLevel(
    const std::string& text,
    const std::string& connector
) const {
    int depth = 0;

    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '(') {
            ++depth;
        } else if (text[i] == ')') {
            --depth;
        }

        if (depth == 0 &&
            text.compare(i, connector.size(), connector) == 0) {
            return i;
        }
    }

    return std::string::npos;
}

bool NaturalLanguageParser::hasOuterParentheses(
    const std::string& text
) const {
    if (text.size() < 2 ||
        text.front() != '(' ||
        text.back() != ')') {
        return false;
    }

    int depth = 0;

    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '(') {
            ++depth;
        } else if (text[i] == ')') {
            --depth;
        }

        if (depth == 0 && i != text.size() - 1) {
            return false;
        }
    }

    return depth == 0;
}

std::unique_ptr<LogicalExpression>
NaturalLanguageParser::parse(const std::string& sentence) {
    std::string text = trim(sentence);

    while (!text.empty() &&
           (text.back() == '.' ||
            text.back() == ',' ||
            text.back() == ';' ||
            text.back() == '!' ||
            text.back() == '?')) {
        text.pop_back();
        text = trim(text);
    }

    if (text.empty()) {
        throw std::invalid_argument("No se puede analizar una oracion vacia");
    }

    return parseExpression(text);
}

std::unique_ptr<LogicalExpression>
NaturalLanguageParser::parseExpression(const std::string& text) {
    std::string cleaned = trim(text);

    while (hasOuterParentheses(cleaned)) {
        cleaned = trim(cleaned.substr(1, cleaned.size() - 2));
    }

    return parseIff(cleaned);
}

std::unique_ptr<LogicalExpression>
NaturalLanguageParser::parseIff(const std::string& text) {
    const std::string connector = " si y solo si ";
    std::size_t position = findTopLevel(text, connector);

    if (position != std::string::npos) {
        std::string left = text.substr(0, position);
        std::string right = text.substr(position + connector.size());

        auto leftExpression = parseExpression(left);
        auto rightExpression = parseExpression(right);

        return std::make_unique<BinaryExpression>(
            LogicalOperator::IFF,
            std::move(leftExpression),
            std::move(rightExpression)
        );
    }

    return parseImplication(text);
}

std::unique_ptr<LogicalExpression>
NaturalLanguageParser::parseImplication(const std::string& text) {
    std::string normalized = normalize(text);

    if (normalized.rfind("si ", 0) == 0) {
        std::string rest = trim(text.substr(3));
        std::string normalizedRest = normalize(rest);

        std::size_t position =
            normalizedRest.find(", entonces ");

        std::size_t separatorSize = 11;

        if (position == std::string::npos) {
            position = normalizedRest.find(" entonces ");
            separatorSize = 10;
        }

        if (position != std::string::npos) {
            std::string left = trim(rest.substr(0, position));
            std::string right = trim(
                rest.substr(position + separatorSize)
            );

            auto leftExpression = parseExpression(left);
            auto rightExpression = parseExpression(right);

            return std::make_unique<BinaryExpression>(
                LogicalOperator::IMPLIES,
                std::move(leftExpression),
                std::move(rightExpression)
            );
        }

        throw std::invalid_argument(
            "Se esperaba 'entonces' en la implicacion"
        );
    }

    const std::string connector = " implica ";
    std::size_t position = findTopLevel(text, connector);

    if (position != std::string::npos) {
        std::string left = text.substr(0, position);
        std::string right = text.substr(position + connector.size());

        auto leftExpression = parseExpression(left);
        auto rightExpression = parseExpression(right);

        return std::make_unique<BinaryExpression>(
            LogicalOperator::IMPLIES,
            std::move(leftExpression),
            std::move(rightExpression)
        );
    }

    return parseOr(text);
}

std::unique_ptr<LogicalExpression>
NaturalLanguageParser::parseOr(const std::string& text) {
    const std::string connector = " o ";
    std::size_t position = findTopLevel(text, connector);

    if (position != std::string::npos) {
        std::string left = text.substr(0, position);
        std::string right = text.substr(position + connector.size());

        auto leftExpression = parseExpression(left);
        auto rightExpression = parseExpression(right);

        return std::make_unique<BinaryExpression>(
            LogicalOperator::OR,
            std::move(leftExpression),
            std::move(rightExpression)
        );
    }

    return parseAnd(text);
}

std::unique_ptr<LogicalExpression>
NaturalLanguageParser::parseAnd(const std::string& text) {
    const std::string connector = " y ";
    std::size_t position = findTopLevel(text, connector);

    if (position != std::string::npos) {
        std::string left = text.substr(0, position);
        std::string right = text.substr(position + connector.size());

        auto leftExpression = parseExpression(left);
        auto rightExpression = parseExpression(right);

        return std::make_unique<BinaryExpression>(
            LogicalOperator::AND,
            std::move(leftExpression),
            std::move(rightExpression)
        );
    }

    return parseUnary(text);
}

std::unique_ptr<LogicalExpression>
NaturalLanguageParser::parseUnary(const std::string& text) {
    std::string expression = trim(text);
    std::string normalized = normalize(expression);

    if (normalized.rfind("no ", 0) == 0) {
        auto operand = parseExpression(expression.substr(3));

        return std::make_unique<UnaryExpression>(
            LogicalOperator::NOT,
            std::move(operand)
        );
    }

    if (normalized.rfind("not ", 0) == 0) {
        auto operand = parseExpression(expression.substr(4));

        return std::make_unique<UnaryExpression>(
            LogicalOperator::NOT,
            std::move(operand)
        );
    }

    if (expression.empty()) {
        throw std::invalid_argument("No se encontro una proposicion");
    }

    std::string symbol =
        knowledgeBase.registerProposition(expression);

    return std::make_unique<AtomicExpression>(symbol);
}

void NaturalLanguageParser::parseAndAddRule(
    const std::string& sentence
) {
    auto expression = parse(sentence);

    knowledgeBase.addRule(
        sentence,
        std::move(expression)
    );
}

