\# LogicTree



\### Sistema de inferencia lógica mediante árboles semánticos



LogicTree es un programa desarrollado en \*\*C++17\*\* que permite representar una base de conocimiento mediante proposiciones y reglas lógicas escritas en lenguaje natural. Utiliza el método de \*\*tablas semánticas (Semantic Tableaux)\*\* para evaluar consultas y determinar si pueden deducirse a partir de la información proporcionada.



\## Objetivo del proyecto



Desarrollar un sistema capaz de interpretar reglas lógicas, construir expresiones proposicionales y comprobar la validez de una consulta mediante la expansión de árboles semánticos.



\## Características principales



\- Registro de proposiciones atómicas.

\- Construcción de una base de conocimiento.

\- Análisis de expresiones lógicas.

\- Interpretación de conectores lógicos:

&#x20; - Negación (NOT).

&#x20; - Conjunción (AND).

&#x20; - Disyunción (OR).

&#x20; - Implicación (IMPLIES).

&#x20; - Bicondicional (IFF).

\- Construcción y expansión de árboles semánticos.

\- Detección de contradicciones.

\- Identificación de ramas abiertas y cerradas.

\- Evaluación de consultas introducidas por el usuario.

\- Clasificación de resultados en tres posibilidades:

&#x20; - SÍ.

&#x20; - NO.

&#x20; - NO SE PUEDE DETERMINAR.



\## Tecnologías utilizadas



\- Lenguaje: C++.

\- Estándar: C++17.

\- Compilador: GCC / MinGW.

\- Entorno de desarrollo: Visual Studio Code o terminal de Windows.

\- Control de versiones: Git y GitHub.



\## Estructura del proyecto



```text

LogicTree/

│

├── include/

│   ├── KnowledgeBase.h

│   ├── LogicalExpression.h

│   ├── NaturalLanguageParser.h

│   ├── Proposition.h

│   └── TableauEngine.h

│

├── src/

│   ├── KnowledgeBase.cpp

│   ├── LogicalExpression.cpp

│   ├── main.cpp

│   ├── NaturalLanguageParser.cpp

│   ├── Proposition.cpp

│   └── TableauEngine.cpp

│

├── .gitignore

├── LICENSE

├── README.md

└── LogicTree.exe

```



\## Funcionamiento



El programa trabaja mediante los siguientes pasos:



1\. El usuario introduce las reglas que conforman la base de conocimiento.

2\. El sistema identifica las proposiciones y construye sus expresiones lógicas.

3\. El usuario introduce una consulta independiente.

4\. El motor de tablas semánticas comprueba la consistencia de la base.

5\. Se evalúa la consulta y su negación.

6\. El sistema determina el resultado y lo muestra en pantalla.



\## Ejemplo de funcionamiento



\### Base de conocimiento



```text

Es de dia

Si es de dia, entonces voy a trabajar

```



\### Consulta



```text

Voy a trabajar

```



\### Resultado esperado



```text

RESPUESTA: SI

La consulta se deduce de la base de conocimiento.

```



\## Interpretación de resultados



| Resultado | Significado |

|---|---|

| SÍ | La consulta se deduce de la base de conocimiento. |

| NO | Se deduce que la consulta es falsa. |

| NO SE PUEDE DETERMINAR | No se puede demostrar la consulta ni su negación. |



Si la base de conocimiento es inconsistente, el programa informa que no es posible evaluar la consulta.



\## Compilación



Para compilar el proyecto utilizando GCC, ejecutar desde la carpeta principal:



```bash

g++ -std=c++17 -Wall -Wextra -mconsole -I include src/main.cpp src/Proposition.cpp src/LogicalExpression.cpp src/KnowledgeBase.cpp src/NaturalLanguageParser.cpp src/TableauEngine.cpp -o LogicTree.exe

```



\## Ejecución



En Windows PowerShell:



```powershell

.\\LogicTree.exe

```



El programa solicitará las reglas de la base de conocimiento. Para finalizar su ingreso, se debe presionar Enter en una línea vacía. Después se podrá introducir la consulta que se desea evaluar.



\## Método lógico utilizado



LogicTree utiliza el método de tablas semánticas para comprobar la satisfacibilidad de las expresiones.



Para determinar si una consulta \\(Q\\) se deduce de una base de conocimiento \\(KB\\), se comprueba:



\\\[

KB \\land \\neg Q

\\]



Si esta expresión no tiene ninguna rama abierta, la consulta se deduce de la base de conocimiento.



También se comprueba la consistencia de la base y la satisfacibilidad de la consulta normal para distinguir entre los tres resultados posibles.



\## Estado del proyecto



\- \[x] Registro de proposiciones.

\- \[x] Base de conocimiento.

\- \[x] Análisis de expresiones lógicas.

\- \[x] Motor de tablas semánticas.

\- \[x] Detección de contradicciones.

\- \[x] Evaluación de consultas independientes.

\- \[x] Pruebas de resultados SÍ, NO y NO SE PUEDE DETERMINAR.



\---



\*\*LogicTree — Proyecto académico de lógica proposicional e inteligencia artificial.\*\*

