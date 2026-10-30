
#include "KnowledgeBase.h"
#include "NaturalLanguageParser.h"
#include "TableauEngine.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

int main() {

    try {

        KnowledgeBase knowledgeBase;
        NaturalLanguageParser parser(knowledgeBase);

        std::cout << "========================================\n";
        std::cout << "          LOGICTREE\n";
        std::cout << "  SISTEMA DE INFERENCIA LOGICA\n";
        std::cout << "========================================\n\n";

        std::cout << "INSTRUCCIONES:\n";
        std::cout << "- Escribe una regla por linea.\n";
        std::cout << "- Deja una linea vacia para terminar las reglas.\n";
        std::cout << "- Despues escribe la consulta que deseas evaluar.\n\n";

        // INGRESAR REGLAS
        std::cout << "========== BASE DE CONOCIMIENTO ==========\n";

        int ruleCount = 0;
        std::string sentence;

        while (true) {

            std::cout << "Regla " << (ruleCount + 1)
                      << " (Enter para terminar): ";

            std::getline(std::cin, sentence);

            if (sentence.empty()) {
                break;
            }

            parser.parseAndAddRule(sentence);
            ++ruleCount;
        }

        if (ruleCount == 0) {
            std::cout << "\nNo ingresaste ninguna regla.\n";
            return 0;
        }

        // MOSTRAR BASE DE CONOCIMIENTO
        std::cout << "\n========== BASE REGISTRADA ==========\n";
        knowledgeBase.display();

        // INGRESAR CONSULTA
        std::cout << "\n========== CONSULTA ==========\n";
        std::cout << "Escribe tu consulta: ";

        std::string queryText;
        std::getline(std::cin, queryText);

        if (queryText.empty()) {
            std::cout << "\nNo ingresaste ninguna consulta.\n";
            return 0;
        }

        std::unique_ptr<LogicalExpression> query =
            parser.parse(queryText);

        // CREAR MOTOR DE INFERENCIA
        TableauEngine tableau(knowledgeBase);

        std::cout << "\nConsulta: " << queryText << '\n';

        // PRUEBA 1: COMPROBAR CONSISTENCIA DE LA BASE
        tableau.run();

        bool baseConsistent = tableau.hasOpenBranch();

        if (!baseConsistent) {

            std::cout << "\n========== RESULTADO ==========\n";
            std::cout << "BASE DE CONOCIMIENTO INCONSISTENTE\n";
            std::cout << "No es posible evaluar la consulta.\n";

        } else {

            // PRUEBA 2: COMPROBAR SI SE DEDUCE LA CONSULTA
            // Se agrega temporalmente la negacion de la consulta.
            tableau.runWithAssumption(*query, true);

            bool negatedQuerySatisfiable =
                tableau.hasOpenBranch();

            // PRUEBA 3: COMPROBAR SI SE DEDUCE LA NEGACION
            // Se agrega temporalmente la consulta.
            tableau.runWithAssumption(*query, false);

            bool querySatisfiable =
                tableau.hasOpenBranch();

            // MOSTRAR RESPUESTA
            std::cout << "\n========== RESULTADO ==========\n";

            if (!negatedQuerySatisfiable) {

                std::cout << "RESPUESTA: SI\n";
                std::cout
                    << "La consulta se deduce de la base de conocimiento.\n";

            } else if (!querySatisfiable) {

                std::cout << "RESPUESTA: NO\n";
                std::cout
                    << "Se deduce que la consulta es falsa.\n";

            } else {

                std::cout << "RESPUESTA: NO SE PUEDE DETERMINAR\n";
                std::cout
                    << "No se puede demostrar la consulta ni su negacion.\n";
            }
        }

        std::cout << "\n========================================\n";
        std::cout << "             FIN DE LOGICTREE\n";
        std::cout << "========================================\n";

    } catch (const std::exception& error) {

        std::cerr << "\nERROR: "
                  << error.what()
                  << '\n';

        return 1;
    }

    return 0;
}
