
#ifndef TABLEAU_ENGINE_H
#define TABLEAU_ENGINE_H

#include "KnowledgeBase.h"

#include <memory>
#include <string>
#include <vector>

struct SignedFormula {
    const LogicalExpression* expression;
    bool negated;
    bool expanded;
};

struct TableauBranch {
    std::vector<SignedFormula> formulas;
    bool closed;
    std::string closureReason;
};

struct TableauNode {
    std::string label;
    std::vector<std::unique_ptr<TableauNode>> children;
};

class TableauEngine {
private:
    const KnowledgeBase& knowledgeBase;

    std::vector<TableauBranch> finalBranches;

    std::unique_ptr<TableauNode> semanticTree;

    bool isContradictory(TableauBranch& branch) const;

    bool isAtomic(
        const LogicalExpression& expression
    ) const;

    int findExpandableFormula(
        const TableauBranch& branch
    ) const;

    std::unique_ptr<TableauNode> expandBranch(
        TableauBranch branch
    );

    void addFormula(
        TableauBranch& branch,
        const LogicalExpression& expression,
        bool negated
    ) const;

    void runInternal(
        const LogicalExpression* assumption,
        bool negated
    );

    void displayBranch(
        const TableauBranch& branch,
        std::size_t branchNumber
    ) const;

    void displayModel(
        const TableauBranch& branch
    ) const;

    std::string formulaToString(
        const SignedFormula& formula
    ) const;

    void displayTreeRecursive(
        const TableauNode& node,
        const std::string& prefix,
        bool isLast
    ) const;

    void displayTree() const;

public:
    explicit TableauEngine(
        const KnowledgeBase& knowledgeBase
    );

    void run();

    void runWithAssumption(
        const LogicalExpression& expression,
        bool negated
    );

    bool hasOpenBranch() const;

    void displayResults() const;
};

#endif
