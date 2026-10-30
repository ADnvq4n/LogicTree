
#include "TableauEngine.h"

#include <iostream>
#include <stdexcept>
#include <utility>

TableauEngine::TableauEngine(
    const KnowledgeBase& knowledgeBase
) : knowledgeBase(knowledgeBase) {
}

// COMPROBAR SI UNA EXPRESION ES ATOMICA
bool TableauEngine::isAtomic(
    const LogicalExpression& expression
) const {
    return dynamic_cast<const AtomicExpression*>(
        &expression
    ) != nullptr;
}

// AGREGAR FORMULA A UNA RAMA
void TableauEngine::addFormula(
    TableauBranch& branch,
    const LogicalExpression& expression,
    bool negated
) const {
    branch.formulas.push_back({
        &expression,
        negated,
        false
    });
}

// DETECTAR CONTRADICCIONES
bool TableauEngine::isContradictory(
    TableauBranch& branch
) const {

    for (std::size_t i = 0; i < branch.formulas.size(); ++i) {

        const auto* atomA =
            dynamic_cast<const AtomicExpression*>(
                branch.formulas[i].expression
            );

        if (!atomA) {
            continue;
        }

        for (std::size_t j = i + 1;
             j < branch.formulas.size(); ++j) {

            const auto* atomB =
                dynamic_cast<const AtomicExpression*>(
                    branch.formulas[j].expression
                );

            if (!atomB) {
                continue;
            }

            if (atomA->getSymbol() == atomB->getSymbol() &&
                branch.formulas[i].negated !=
                branch.formulas[j].negated) {

                branch.closed = true;

                branch.closureReason =
                    atomA->getSymbol() +
                    " y NOT(" +
                    atomA->getSymbol() +
                    ")";

                return true;
            }
        }
    }

    return false;
}

// BUSCAR FORMULA PENDIENTE DE EXPANSION
int TableauEngine::findExpandableFormula(
    const TableauBranch& branch
) const {

    for (std::size_t i = 0;
         i < branch.formulas.size(); ++i) {

        if (branch.formulas[i].expanded) {
            continue;
        }

        if (!isAtomic(*branch.formulas[i].expression)) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// EXPANDIR RAMAS DEL ARBOL SEMANTICO
std::unique_ptr<TableauNode>
TableauEngine::expandBranch(TableauBranch branch) {

    if (isContradictory(branch)) {

        finalBranches.push_back(branch);

        auto node = std::make_unique<TableauNode>();
        node->label =
            "RAMA CERRADA: " + branch.closureReason;

        return node;
    }

    int index = findExpandableFormula(branch);

    if (index == -1) {

        finalBranches.push_back(branch);

        auto node = std::make_unique<TableauNode>();
        node->label = "RAMA ABIERTA";

        return node;
    }

    SignedFormula current = branch.formulas[index];

    branch.formulas[index].expanded = true;

    auto node = std::make_unique<TableauNode>();

    node->label =
        "Aplicar: " + formulaToString(current);

    // NEGACION
    if (const auto* unary =
        dynamic_cast<const UnaryExpression*>(
            current.expression
        )) {

        TableauBranch nextBranch = branch;

        addFormula(
            nextBranch,
            unary->getOperand(),
            !current.negated
        );

        node->children.push_back(
            expandBranch(std::move(nextBranch))
        );

        return node;
    }

    const auto* binary =
        dynamic_cast<const BinaryExpression*>(
            current.expression
        );

    if (!binary) {
        throw std::runtime_error(
            "No se pudo interpretar la formula"
        );
    }

    const auto& left = binary->getLeft();
    const auto& right = binary->getRight();

    LogicalOperator operation =
        binary->getOperator();

    bool negative = current.negated;

    // CONJUNCION: A AND B
    if (operation == LogicalOperator::AND) {

        if (!negative) {

            TableauBranch nextBranch = branch;

            addFormula(nextBranch, left, false);
            addFormula(nextBranch, right, false);

            node->label = "A Y B";

            node->children.push_back(
                expandBranch(std::move(nextBranch))
            );

        } else {

            TableauBranch first = branch;
            TableauBranch second = branch;

            addFormula(first, left, true);
            addFormula(second, right, true);

            node->label = "NOT(A) O NOT(B)";

            node->children.push_back(
                expandBranch(std::move(first))
            );

            node->children.push_back(
                expandBranch(std::move(second))
            );
        }

        return node;
    }

    // DISYUNCION: A OR B
    if (operation == LogicalOperator::OR) {

        if (!negative) {

            TableauBranch first = branch;
            TableauBranch second = branch;

            addFormula(first, left, false);
            addFormula(second, right, false);

            node->label = "A O B";

            node->children.push_back(
                expandBranch(std::move(first))
            );

            node->children.push_back(
                expandBranch(std::move(second))
            );

        } else {

            TableauBranch nextBranch = branch;

            addFormula(nextBranch, left, true);
            addFormula(nextBranch, right, true);

            node->label = "NOT(A) Y NOT(B)";

            node->children.push_back(
                expandBranch(std::move(nextBranch))
            );
        }

        return node;
    }

    // IMPLICACION: A -> B
    if (operation == LogicalOperator::IMPLIES) {

        if (!negative) {

            TableauBranch first = branch;
            TableauBranch second = branch;

            addFormula(first, left, true);
            addFormula(second, right, false);

            node->label = "NOT(A) O B";

            node->children.push_back(
                expandBranch(std::move(first))
            );

            node->children.push_back(
                expandBranch(std::move(second))
            );

        } else {

            TableauBranch nextBranch = branch;

            addFormula(nextBranch, left, false);
            addFormula(nextBranch, right, true);

            node->label = "A Y NOT(B)";

            node->children.push_back(
                expandBranch(std::move(nextBranch))
            );
        }

        return node;
    }

    // BICONDICIONAL: A <-> B
    if (operation == LogicalOperator::IFF) {

        TableauBranch first = branch;
        TableauBranch second = branch;

        if (!negative) {

            addFormula(first, left, false);
            addFormula(first, right, false);

            addFormula(second, left, true);
            addFormula(second, right, true);

            node->label =
                "(A Y B) O (NOT(A) Y NOT(B))";

        } else {

            addFormula(first, left, false);
            addFormula(first, right, true);

            addFormula(second, left, true);
            addFormula(second, right, false);

            node->label =
                "(A Y NOT(B)) O (NOT(A) Y B)";
        }

        node->children.push_back(
            expandBranch(std::move(first))
        );

        node->children.push_back(
            expandBranch(std::move(second))
        );

        return node;
    }

    throw std::runtime_error(
        "Operador logico no reconocido"
    );
}

// EJECUTAR BASE DE CONOCIMIENTO NORMAL
void TableauEngine::run() {
    runInternal(nullptr, false);
}

// EJECUTAR CON UNA CONSULTA O SU NEGACION
void TableauEngine::runWithAssumption(
    const LogicalExpression& expression,
    bool negated
) {
    runInternal(&expression, negated);
}

// COMPROBAR SI EXISTE UNA RAMA ABIERTA
bool TableauEngine::hasOpenBranch() const {

    for (const auto& branch : finalBranches) {

        if (!branch.closed) {
            return true;
        }
    }

    return false;
}

// EJECUCION INTERNA DEL TABLEAU
void TableauEngine::runInternal(
    const LogicalExpression* assumption,
    bool negated
) {

    finalBranches.clear();
    semanticTree.reset();

    if (knowledgeBase.isEmpty()) {
        throw std::runtime_error(
            "La base de conocimiento no contiene reglas"
        );
    }

    TableauBranch initialBranch{};
    initialBranch.closed = false;

    // AGREGAR TODAS LAS REGLAS
    for (const auto& rule : knowledgeBase.getRules()) {

        addFormula(
            initialBranch,
            rule.getExpression(),
            false
        );
    }

    // AGREGAR CONSULTA TEMPORAL
    if (assumption != nullptr) {

        addFormula(
            initialBranch,
            *assumption,
            negated
        );
    }

    semanticTree = std::make_unique<TableauNode>();
    semanticTree->label = "INICIO";

    semanticTree->children.push_back(
        expandBranch(std::move(initialBranch))
    );
}

// CONVERTIR FORMULA A TEXTO
std::string TableauEngine::formulaToString(
    const SignedFormula& formula
) const {

    std::string result =
        formula.expression->toString();

    if (formula.negated) {
        return "NOT(" + result + ")";
    }

    return result;
}

// MOSTRAR RAMA
void TableauEngine::displayBranch(
    const TableauBranch& branch,
    std::size_t branchNumber
) const {

    std::cout << "\nRama " << branchNumber << '\n';
    std::cout << "-----------------------------\n";

    for (const auto& formula : branch.formulas) {

        std::cout << formulaToString(formula);

        if (formula.expanded) {
            std::cout << "  [Aplicada]";
        }

        std::cout << '\n';
    }

    if (branch.closed) {

        std::cout << "Resultado: RAMA CERRADA (X)\n";

        std::cout << "Contradiccion: "
                  << branch.closureReason
                  << '\n';

    } else {

        std::cout << "Resultado: RAMA ABIERTA\n";
    }
}

// MOSTRAR MODELO
void TableauEngine::displayModel(
    const TableauBranch& branch
) const {

    std::cout << "Modelo: ";

    for (const auto& item :
         knowledgeBase.getPropositions()) {

        const std::string& symbol = item.first;

        bool found = false;
        bool value = false;

        for (const auto& formula : branch.formulas) {

            const auto* atom =
                dynamic_cast<const AtomicExpression*>(
                    formula.expression
                );

            if (atom && atom->getSymbol() == symbol) {

                found = true;
                value = !formula.negated;

                break;
            }
        }

        if (!found) {
            value = false;
        }

        std::cout << symbol << '='
                  << (value ? 'V' : 'F')
                  << "  ";
    }

    std::cout << '\n';
}

// MOSTRAR ARBOL RECURSIVAMENTE
void TableauEngine::displayTreeRecursive(
    const TableauNode& node,
    const std::string& prefix,
    bool isLast
) const {

    std::cout << prefix;

    if (prefix.empty()) {
        std::cout << node.label << '\n';
    } else {

        std::cout << (isLast ? "`-- " : "|-- ")
                  << node.label
                  << '\n';
    }

    std::string nextPrefix = prefix;

    if (!prefix.empty()) {
        nextPrefix += isLast ? "    " : "|   ";
    }

    for (std::size_t i = 0;
         i < node.children.size(); ++i) {

        bool last = (i == node.children.size() - 1);

        displayTreeRecursive(
            *node.children[i],
            nextPrefix,
            last
        );
    }
}

// MOSTRAR ARBOL
void TableauEngine::displayTree() const {

    if (!semanticTree) {
        std::cout << "No hay arbol semantico disponible.\n";
        return;
    }

    std::cout << "\n========== ARBOL SEMANTICO ==========\n";

    displayTreeRecursive(
        *semanticTree,
        "",
        true
    );
}

// MOSTRAR RESULTADOS
void TableauEngine::displayResults() const {

    displayTree();

    std::cout << "\n========== RESULTADOS DE LAS RAMAS ==========\n";

    bool openBranchFound = false;
    std::size_t openCount = 0;

    for (std::size_t i = 0;
         i < finalBranches.size(); ++i) {

        displayBranch(finalBranches[i], i + 1);

        if (!finalBranches[i].closed) {

            openBranchFound = true;
            ++openCount;

            displayModel(finalBranches[i]);
        }
    }

    std::cout << "\n========== CONCLUSION ==========\n";

    if (openBranchFound) {

        std::cout << "La base de conocimiento es consistente.\n";

        std::cout << "Ramas abiertas: "
                  << openCount
                  << '\n';

    } else {

        std::cout << "La base de conocimiento es inconsistente.\n";

        std::cout << "Todas las ramas se cerraron.\n";
    }
}
