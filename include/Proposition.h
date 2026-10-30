#ifndef PROPOSITION_H
#define PROPOSITION_H

#include <string>

class Proposition {
private:
    std::string symbol;
    std::string description;

public:
    Proposition(const std::string& symbol,
                const std::string& description);

    const std::string& getSymbol() const;
    const std::string& getDescription() const;
};

#endif
