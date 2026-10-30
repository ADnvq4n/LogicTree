#ifndef NATURAL_LANGUAGE_PARSER_H
#define NATURAL_LANGUAGE_PARSER_H

#include "KnowledgeBase.h"

#include <memory>
#include <string>

class NaturalLanguageParser {
private:
    KnowledgeBase& knowledgeBase;

    std::string trim(const std::string& text) const;
    std::string normalize(const std::string& text) const;

    std::size_t findTopLevel(
        const std::string& text,
        const std::string& connector
    ) const;

    bool hasOuterParentheses(const std::string& text) const;

    std::unique_ptr<LogicalExpression> parseExpression(
        const std::string& text
    );

    std::unique_ptr<LogicalExpression> parseIff(const std::string& text);
    std::unique_ptr<LogicalExpression> parseImplication(const std::string& text);
    std::unique_ptr<LogicalExpression> parseOr(const std::string& text);
    std::unique_ptr<LogicalExpression> parseAnd(const std::string& text);
    std::unique_ptr<LogicalExpression> parseUnary(const std::string& text);

public:
    explicit NaturalLanguageParser(KnowledgeBase& knowledgeBase);

    std::unique_ptr<LogicalExpression> parse(const std::string& sentence);
    void parseAndAddRule(const std::string& sentence);
};

#endif
