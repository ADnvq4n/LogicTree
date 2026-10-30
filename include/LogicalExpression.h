#ifndef LOGICAL_EXPRESSION_H
#define LOGICAL_EXPRESSION_H

#include <memory>
#include <string>

enum class LogicalOperator {
    NOT,
    AND,
    OR,
    IMPLIES,
    IFF
};

class LogicalExpression {
public:
    virtual ~LogicalExpression() = default;
    virtual std::string toString() const = 0;
};

class AtomicExpression : public LogicalExpression {
private:
    std::string symbol;

public:
    explicit AtomicExpression(const std::string& symbol);
    const std::string& getSymbol() const;
    std::string toString() const override;
};

class UnaryExpression : public LogicalExpression {
private:
    LogicalOperator operation;
    std::unique_ptr<LogicalExpression> operand;

public:
    UnaryExpression(
        LogicalOperator operation,
        std::unique_ptr<LogicalExpression> operand
    );

    LogicalOperator getOperator() const;
    const LogicalExpression& getOperand() const;
    std::string toString() const override;
};

class BinaryExpression : public LogicalExpression {
private:
    LogicalOperator operation;
    std::unique_ptr<LogicalExpression> left;
    std::unique_ptr<LogicalExpression> right;

public:
    BinaryExpression(
        LogicalOperator operation,
        std::unique_ptr<LogicalExpression> left,
        std::unique_ptr<LogicalExpression> right
    );

    LogicalOperator getOperator() const;
    const LogicalExpression& getLeft() const;
    const LogicalExpression& getRight() const;
    std::string toString() const override;
};

#endif
