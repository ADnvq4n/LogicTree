#include "LogicalExpression.h"

#include <stdexcept>
#include <utility>

static std::string operatorSymbol(LogicalOperator operation) {
    switch (operation) {
        case LogicalOperator::NOT: return "NOT";
        case LogicalOperator::AND: return "AND";
        case LogicalOperator::OR: return "OR";
        case LogicalOperator::IMPLIES: return "->";
        case LogicalOperator::IFF: return "<->";
    }

    throw std::invalid_argument("Operador logico desconocido");
}

AtomicExpression::AtomicExpression(const std::string& symbol)
    : symbol(symbol) {
    if (symbol.empty()) {
        throw std::invalid_argument("El atomo no puede estar vacio");
    }
}

const std::string& AtomicExpression::getSymbol() const {
    return symbol;
}

std::string AtomicExpression::toString() const {
    return symbol;
}

UnaryExpression::UnaryExpression(
    LogicalOperator operation,
    std::unique_ptr<LogicalExpression> operand
) : operation(operation), operand(std::move(operand)) {
    if (operation != LogicalOperator::NOT || !this->operand) {
        throw std::invalid_argument("Expresion unaria invalida");
    }
}

LogicalOperator UnaryExpression::getOperator() const {
    return operation;
}

const LogicalExpression& UnaryExpression::getOperand() const {
    return *operand;
}

std::string UnaryExpression::toString() const {
    return operatorSymbol(operation) + "(" +
           operand->toString() + ")";
}

BinaryExpression::BinaryExpression(
    LogicalOperator operation,
    std::unique_ptr<LogicalExpression> left,
    std::unique_ptr<LogicalExpression> right
) : operation(operation),
    left(std::move(left)),
    right(std::move(right)) {
    if (operation == LogicalOperator::NOT ||
        !this->left || !this->right) {
        throw std::invalid_argument("Expresion binaria invalida");
    }
}

LogicalOperator BinaryExpression::getOperator() const {
    return operation;
}

const LogicalExpression& BinaryExpression::getLeft() const {
    return *left;
}

const LogicalExpression& BinaryExpression::getRight() const {
    return *right;
}

std::string BinaryExpression::toString() const {
    return "(" + left->toString() + " " +
           operatorSymbol(operation) + " " +
           right->toString() + ")";
}
