LogicTree

Sistema de inferencia lógica mediante árboles semánticos

LogicTree es un programa desarrollado en C++17 que permite representar una base de conocimiento mediante proposiciones y reglas lógicas escritas en lenguaje natural. Utiliza el método de tablas semánticas (Semantic Tableaux) para evaluar consultas y determinar si pueden deducirse a partir de la información proporcionada.

Objetivo del proyecto

Desarrollar un sistema capaz de interpretar reglas lógicas, construir expresiones proposicionales y comprobar la validez de una consulta mediante la expansión de árboles semánticos.

Características principales

Registro de proposiciones atómicas.

Construcción de una base de conocimiento.

Análisis de expresiones lógicas.

Interpretación de conectores lógicos:

Negación (NOT).

Conjunción (AND).

Disyunción (OR).

Implicación (IMPLIES).

Bicondicional (IFF).

Construcción y ampliación de árboles semánticos.

Detección de contradicciones.

Identificación de ramas abiertas y cerradas.

Evaluación de consultas introducidas por el usuario.

Clasificación de resultados en tres posibilidades:

SÍ.

NO.

NO SE PUEDE DETERMINAR.

Tecnologías utilizadas

Lenguaje: C++.

Estándar: C++17.

Compilador: GCC / MinGW.

Entorno de desarrollo: Visual Studio Code o terminal de Windows.

Control de versiones: Git y GitHub.

Estructura del proyecto

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

Funcionamiento

El programa funciona mediante los siguientes pasos:

El usuario introduce las reglas que conforman la base de conocimiento.

El sistema identifica las proposiciones y construye sus expresiones lógicas.

El usuario introduce una consulta independiente.

El motor de tablas semánticas comprueba la consistencia de la base.

Se evalúa la consulta y su negación.

El sistema determina el resultado y lo muestra en pantalla.

Ejemplo de funcionamiento

Base de conocimiento

Es de dia

Si es de dia, entonces voy a trabajar

Consulta

Voy a trabajar

Resultado esperado

RESPUESTA: SI

La consulta se deduce de la base de conocimiento.

Interpretación de resultados

Resultado

Significado

SÍ

La consulta se deduce de la base de conocimiento.

NO

Se deduce que la consulta es falsa.

NO SE PUEDE DETERMINAR

No se puede demostrar la consulta ni su negación.

Si la base de conocimiento es inconsistente, el programa informa que no es posible evaluar la consulta.

Imágenes del proyecto

En esta sección se pueden consultar capturas de pantalla de la interfaz y del funcionamiento de LogicTree.

<img width="979" height="453" alt="image" src="https://github.com/user-attachments/assets/fa1f6eea-2547-4266-9795-5cbfb49e5baf" />
<img width="986" height="208" alt="image" src="https://github.com/user-attachments/assets/fa1cb0fe-9cc7-4adb-bb3b-a6b93e88f26b" />

<img width="965" height="452" alt="image" src="https://github.com/user-attachments/assets/5c919b93-8475-40d4-af54-9e49bc3021af" />
<img width="977" height="208" alt="image" src="https://github.com/user-attachments/assets/a8f1406c-cccd-4e7f-b869-f11bb482f418" />

Demostración en video

En el siguiente video se presenta el funcionamiento del sistema de inferencia lógica LogicTree, desarrollado en C++17.

https://drive.google.com/file/d/19b3ZuHXPAYyUlXaya_wkmR1st-hhLpve/view?usp=sharing 

Compilación

Para compilar el proyecto utilizando GCC, ejecuta el siguiente comando desde la carpeta principal:

g++ -std=c++17 -Wall -Wextra -mconsole -I include src/main.cpp src/Proposition.cpp src/LogicalExpression.cpp src/KnowledgeBase.cpp src/NaturalLanguageParser.cpp src/TableauEngine.cpp -o LogicTree.exe

Ejecución

En Windows PowerShell:

.\LogicTree.exe

El programa solicitará las reglas de la base de conocimiento. Para finalizar su ingreso, se debe presionar Enter en una línea vacía. Después se podrá introducir la consulta que se desea evaluar.
