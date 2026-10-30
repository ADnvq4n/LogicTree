#ifndef KNOWLEDGE_BASE_H
#define KNOWLEDGE_BASE_H

#include "Proposition.h"
#include "LogicalExpression.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class KnowledgeRule {
private:
    std::string identifier;
    std::string naturalLanguage;
    std::unique_ptr<LogicalExpression> expression;

public:
    KnowledgeRule(
        const std::string& identifier,
        const std::string& naturalLanguage,
        std::unique_ptr<LogicalExpression> expression
    );

    const std::string& getIdentifier() const;
    const std::string& getNaturalLanguage() const;
    const LogicalExpression& getExpression() const;
};

class KnowledgeBase {
private:
    std::map<std::string, Proposition> propositions;
    std::map<std::string, std::string> descriptionIndex;
    std::vector<KnowledgeRule> rules;

    int nextAtom;

    std::string normalizeDescription(
        const std::string& description
    ) const;

    std::string generateSymbol();

public:
    KnowledgeBase();

    std::string registerProposition(const std::string& description);
    const Proposition* findProposition(const std::string& description) const;

    void addRule(
        const std::string& naturalLanguage,
        std::unique_ptr<LogicalExpression> expression
    );

    const std::map<std::string, Proposition>& getPropositions() const;
    const std::vector<KnowledgeRule>& getRules() const;

    void displayAtoms() const;
    void displayRules() const;
    void display() const;

    bool isEmpty() const;
};

#endif
