# Sistema de Sucesión - Familia de la Mafia

Este programa gestiona la sucesión jerárquica de una familia mafiosa mediante una estructura de árbol binario, aplicando reglas automáticas cuando el jefe fallece, cae en prisión o supera los 70 años.

## Estructura del Proyecto
- `bin/`: Contiene el archivo de datos `datos.csv` y el ejecutable del sistema.
- `src/`: Contiene todo el código fuente C++ (`.h` y `.cpp`).

## Requisitos de Compilación
Para compilar con G++ desde la raíz del proyecto:

```bash
g++ -std=c++11 src/main.cpp src/arbol.cpp -o bin/mafia_app