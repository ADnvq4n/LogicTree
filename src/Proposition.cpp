#include "Proposition.h"

Proposition::Proposition(
    const std::string& symbol,
    const std::string& description
) : symbol(symbol), description(description) {
}

const std::string& Proposition::getSymbol() const {
    return symbol;
}

const std::string& Proposition::getDescription() const {
    return description;
}
