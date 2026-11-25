#include "arbol.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

ArbolMafia::ArbolMafia() : raiz(nullptr) {}

ArbolMafia::~ArbolMafia() {
    destruirArbol(raiz);
}

void ArbolMafia::destruirArbol(NodoArbol* nodo) {
    if (nodo != nullptr) {
        destruirArbol(nodo->izq);
        destruirArbol(nodo->der);
        delete nodo;
    }
}

void ArbolMafia::cargarDatosPorDefecto() {
    cout << "Cargando datos iniciales por defecto..." << endl;
    insertarEnArbol({1, "Vito", "Corleone", 'H', 68, -1, 0, 0, 1, 1});
    insertarEnArbol({2, "Santino", "Corleone", 'H', 40, 1, 0, 0, 0, 0});
    insertarEnArbol({3, "Michael", "Corleone", 'H', 38, 1, 0, 0, 0, 0});
    insertarEnArbol({4, "Fredo", "Corleone", 'H', 42, 2, 0, 0, 0, 0});
    insertarEnArbol({5, "Vincent", "Mancini", 'H', 25, 3, 0, 0, 0, 0});
}

NodoArbol* ArbolMafia::buscarNodoRec(NodoArbol* nodo, int id) {
    if (nodo == nullptr) return nullptr;
    if (nodo->persona.id == id) return nodo;

    NodoArbol* encontradoIzq = buscarNodoRec(nodo->izq, id);
    if (encontradoIzq != nullptr) return encontradoIzq;

    return buscarNodoRec(nodo->der, id);
}

void ArbolMafia::cargarDesdeCSV(const string& rutaArchivo) {
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir '" << rutaArchivo << "'." << endl;
        cargarDatosPorDefecto();
        return;
    }

    string linea;
    getline(archivo, linea); // Saltar cabecera

    int cargados = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string item;
        Persona p;

        getline(ss, item, ','); p.id = stoi(item);
        getline(ss, p.name, ',');
        getline(ss, p.last_name, ',');
        getline(ss, item, ','); p.gender = item.empty() ? 'H' : item[0];
        getline(ss, item, ','); p.age = stoi(item);
        getline(ss, item, ','); p.id_boss = stoi(item);
        getline(ss, item, ','); p.is_dead = stoi(item);
        getline(ss, item, ','); p.in_jail = stoi(item);
        getline(ss, item, ','); p.was_boss = stoi(item);
        getline(ss, item, ','); p.is_boss = stoi(item);

        insertarEnArbol(p);
        cargados++;
    }
    archivo.close();

    if (cargados == 0) {
        cargarDatosPorDefecto();
    } else {
        cout << "Datos cargados exitosamente (" << cargados << " miembros)." << endl;
    }
}

void ArbolMafia::insertarEnArbol(Persona p) {
    if (buscarNodoRec(raiz, p.id) != nullptr) return;

    NodoArbol* nuevo = new NodoArbol(p);
    if (raiz == nullptr) {
        raiz = nuevo;
        return;
    }

    NodoArbol* padre = buscarNodoRec(raiz, p.id_boss);
    if (padre != nullptr) {
        if (padre->izq == nullptr) {
            padre->izq = nuevo;
        } else if (padre->der == nullptr) {
            padre->der = nuevo;
        } else {
            cout << "El jefe con ID " << p.id_boss << " ya tiene dos sucesores directos." << endl;
            delete nuevo;
        }
    } else {
        delete nuevo;
    }
}

int ArbolMafia::obtenerAltura(NodoArbol* nodo) {
    if (nodo == nullptr) return 0;
    int izq = obtenerAltura(nodo->izq);
    int der = obtenerAltura(nodo->der);
    return 1 + (izq > der ? izq : der);
}

static string etiquetaNodo(NodoArbol* nodo) {
    if (nodo == nullptr || nodo->persona.is_dead == 1) return "";
    string tag = "[" + to_string(nodo->persona.id) + " " + nodo->persona.name + "]";
    if (nodo->persona.is_boss == 1) tag += "*";
    if (nodo->persona.in_jail == 1) tag += "#";
    return tag;
}

void ArbolMafia::mostrarArbolPiramide(NodoArbol* nodo) {
    if (nodo == nullptr) {
        cout << "No hay miembros en el arbol." << endl;
        return;
    }

    int h = obtenerAltura(nodo);
    int anchoCelda = 16;
    int maxNodosNivel = 1;
    for (int i = 1; i < h; i++) maxNodosNivel *= 2;
    int anchoTotal = maxNodosNivel * anchoCelda;
    if (anchoTotal > 120) anchoTotal = 120;

    int capacidad = (1 << h);
    NodoArbol** nivelActual = new NodoArbol*[capacidad];
    int cantActual = 1;
    nivelActual[0] = nodo;

    for (int nivel = 0; nivel < h; nivel++) {
        int separacion = anchoTotal / (cantActual + 1);
        if (separacion < 4) separacion = 4;

        // 1. Imprimir nodos del nivel actual
        int posAnterior = 0;
        for (int i = 0; i < cantActual; i++) {
            int centro = separacion * (i + 1);
            string texto = etiquetaNodo(nivelActual[i]);
            int inicio = centro - (int)texto.size() / 2;
            if (inicio < posAnterior + 1) inicio = posAnterior + 1;

            for (int s = posAnterior; s < inicio; s++) cout << ' ';
            cout << texto;
            posAnterior = inicio + (int)texto.size();
        }
        cout << endl;

        // 2. Imprimir conectores '/' y '\' hacia el siguiente nivel
        if (nivel < h - 1) {
            posAnterior = 0;
            for (int i = 0; i < cantActual; i++) {
                NodoArbol* actual = nivelActual[i];
                int centroPadre = separacion * (i + 1);

                if (actual != nullptr && actual->izq != nullptr && actual->izq->persona.is_dead == 0) {
                    int posSlash = centroPadre - 2;
                    if (posSlash < posAnterior + 1) posSlash = posAnterior + 1;
                    for (int s = posAnterior; s < posSlash; s++) cout << ' ';
                    cout << '/';
                    posAnterior = posSlash + 1;
                }

                if (actual != nullptr && actual->der != nullptr && actual->der->persona.is_dead == 0) {
                    int posBackslash = centroPadre + 2;
                    if (posBackslash < posAnterior + 1) posBackslash = posAnterior + 1;
                    for (int s = posAnterior; s < posBackslash; s++) cout << ' ';
                    cout << '\\';
                    posAnterior = posBackslash + 1;
                }
            }
            cout << endl;
        }

        // 3. Preparar siguiente nivel
        NodoArbol** siguienteNivel = new NodoArbol*[capacidad];
        int cantSiguiente = 0;
        for (int i = 0; i < cantActual; i++) {
            NodoArbol* actual = nivelActual[i];
            siguienteNivel[cantSiguiente++] = (actual != nullptr) ? actual->izq : nullptr;
            siguienteNivel[cantSiguiente++] = (actual != nullptr) ? actual->der : nullptr;
        }

        delete[] nivelActual;
        nivelActual = siguienteNivel;
        cantActual = cantSiguiente;
    }

    delete[] nivelActual;
    cout << "\nLeyenda: * = Jefe actual | # = En carcel" << endl;
}

void ArbolMafia::mostrarLineaSucesion() {
    cout << "\n=== ARBOL DE SUCESION ACTUAL (SOLO VIVOS) ===" << endl;
    mostrarArbolPiramide(raiz);
}

NodoArbol* ArbolMafia::buscarPrimerSucesorValido(NodoArbol* subraiz, bool incluirEnCarcel) {
    if (subraiz == nullptr) return nullptr;

    if (subraiz->persona.is_dead == 0) {
        if (incluirEnCarcel || subraiz->persona.in_jail == 0) {
            return subraiz;
        }
    }

    NodoArbol* izq = buscarPrimerSucesorValido(subraiz->izq, incluirEnCarcel);
    if (izq != nullptr) return izq;

    return buscarPrimerSucesorValido(subraiz->der, incluirEnCarcel);
}

void ArbolMafia::verificarYActualizarJefe() {
    if (raiz == nullptr) return;

    if (raiz->persona.is_dead == 1 || raiz->persona.in_jail == 1 || raiz->persona.age > 70) {
        cout << "\n[ALERTA DE SUCESION] El jefe actual no puede seguir al mando." << endl;

        NodoArbol* nuevoJefe = nullptr;

        if (raiz->izq) nuevoJefe = buscarPrimerSucesorValido(raiz->izq, false);
        if (!nuevoJefe && raiz->der) nuevoJefe = buscarPrimerSucesorValido(raiz->der, false);

        if (!nuevoJefe) {
            if (raiz->izq) nuevoJefe = buscarPrimerSucesorValido(raiz->izq, true);
            if (!nuevoJefe && raiz->der) nuevoJefe = buscarPrimerSucesorValido(raiz->der, true);
        }

        if (nuevoJefe != nullptr) {
            raiz->persona.is_boss = 0;
            raiz->persona.was_boss = 1;
            nuevoJefe->persona.is_boss = 1;

            cout << "El nuevo Jefe de la familia es: "
                 << nuevoJefe->persona.name << " " << nuevoJefe->persona.last_name
                 << " (ID: " << nuevoJefe->persona.id << ")" << endl;
        } else {
            cout << "Sin sucesores disponibles para asumir el mando." << endl;
        }
    }
}

void ArbolMafia::editarMiembro(int id) {
    NodoArbol* nodo = buscarNodoRec(raiz, id);
    if (nodo == nullptr) {
        cout << "Miembro no encontrado." << endl;
        return;
    }

    cout << "\nEditando a: " << nodo->persona.name << " " << nodo->persona.last_name << endl;
    cout << "Nuevo Nombre: "; cin >> nodo->persona.name;
    cout << "Nuevo Apellido: "; cin >> nodo->persona.last_name;
    cout << "Nueva Edad: "; cin >> nodo->persona.age;
    cout << "Esta muerto? (1 Si / 0 No): "; cin >> nodo->persona.is_dead;
    cout << "Esta en la carcel? (1 Si / 0 No): "; cin >> nodo->persona.in_jail;

    verificarYActualizarJefe();
}