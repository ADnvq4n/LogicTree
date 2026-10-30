#include "KnowledgeBase.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <utility>

KnowledgeRule::KnowledgeRule(
    const std::string& identifier,
    const std::string& naturalLanguage,
    std::unique_ptr<LogicalExpression> expression
)
    : identifier(identifier),
      naturalLanguage(naturalLanguage),
      expression(std::move(expression)) {

    if (!this->expression) {
        throw std::invalid_argument("La regla necesita una expresion logica");
    }
}

const std::string& KnowledgeRule::getIdentifier() const {
    return identifier;
}

const std::string& KnowledgeRule::getNaturalLanguage() const {
    return naturalLanguage;
}

const LogicalExpression& KnowledgeRule::getExpression() const {
    return *expression;
}

KnowledgeBase::KnowledgeBase() : nextAtom(0) {
}

std::string KnowledgeBase::normalizeDescription(
    const std::string& description
) const {
    std::string result;
    bool previousSpace = false;

    for (unsigned char character : description) {
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

    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }

    return result;
}

std::string KnowledgeBase::generateSymbol() {
    if (nextAtom < 11) {
        std::string symbols = "pqrstuvwxyz";
        return std::string(1, symbols[nextAtom++]);
    }

    int index = nextAtom++ - 11;
    return "p" + std::to_string(index + 1);
}

std::string KnowledgeBase::registerProposition(
    const std::string& description
) {
    std::string normalized = normalizeDescription(description);

    if (normalized.empty()) {
        throw std::invalid_argument("La proposicion no puede estar vacia");
    }

    auto existing = descriptionIndex.find(normalized);

    if (existing != descriptionIndex.end()) {
        return existing->second;
    }

    std::string symbol = generateSymbol();

    propositions.emplace(
        symbol,
        Proposition(symbol, description)
    );

    descriptionIndex.emplace(normalized, symbol);

    return symbol;
}

const Proposition* KnowledgeBase::findProposition(
    const std::string& description
) const {
    std::string normalized = normalizeDescription(description);
    auto index = descriptionIndex.find(normalized);

    if (index == descriptionIndex.end()) {
        return nullptr;
    }

    auto proposition = propositions.find(index->second);

    if (proposition == propositions.end()) {
        return nullptr;
    }

    return &proposition->second;
}

void KnowledgeBase::addRule(
    const std::string& naturalLanguage,
    std::unique_ptr<LogicalExpression> expression
) {
    if (!expression) {
        throw std::invalid_argument("No se puede agregar una regla vacia");
    }

    std::string identifier = "R" + std::to_string(rules.size() + 1);

    rules.emplace_back(
        identifier,
        naturalLanguage,
        std::move(expression)
    );
}

const std::map<std::string, Proposition>&
KnowledgeBase::getPropositions() const {
    return propositions;
}

const std::vector<KnowledgeRule>& KnowledgeBase::getRules() const {
    return rules;
}

void KnowledgeBase::displayAtoms() const {
    std::cout << "\n========== TABLA DE ATOMOS ==========\n";

    for (const auto& item : propositions) {
        std::cout << item.first << " = "
                  << item.second.getDescription() << '\n';
    }
}

void KnowledgeBase::displayRules() const {
    std::cout << "\n========== REGLAS LOGICAS ==========\n";

    for (const auto& rule : rules) {
        std::cout << rule.getIdentifier() << ": "
                  << rule.getNaturalLanguage()
                  << "\n    "
                  << rule.getExpression().toString()
                  << '\n';
    }
}

void KnowledgeBase::display() const {
    displayAtoms();
    displayRules();
}

bool KnowledgeBase::isEmpty() const {
    return rules.empty();
}
