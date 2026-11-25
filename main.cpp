#include <iostream>
#include "arbol.h"

using namespace std;

int main() {
    ArbolMafia mafia;
    mafia.cargarDesdeCSV("bin/datos.csv");
    mafia.verificarYActualizarJefe();
    mafia.mostrarLineaSucesion(); // Muestra el árbol inicial apenas arranca

    int opcion = 0;
    while (true) {
        cout << "\n========== SISTEMA DE SUCESION DE LA MAFIA ==========" << endl;
        cout << "1. Mostrar arbol de sucesion (vivos)" << endl;
        cout << "2. Editar datos de un miembro" << endl;
        cout << "3. Verificar / Actualizar sucesion del Jefe" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            mafia.mostrarLineaSucesion();
        } else if (opcion == 2) {
            int id;
            cout << "Ingrese el ID del miembro a modificar: ";
            cin >> id;
            mafia.editarMiembro(id);
        } else if (opcion == 3) {
            mafia.verificarYActualizarJefe();
        } else if (opcion == 4) {
            cout << "Saliendo del sistema..." << endl;
            break;
        } else {
            cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    }

    return 0;
}