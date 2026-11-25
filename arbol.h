#ifndef ARBOL_H
#define ARBOL_H
#include "persona.h"
#include <string>
using namespace std;

class ArbolMafia {
private:
    NodoArbol* raiz;

    void destruirArbol(NodoArbol* nodo);
    int obtenerAltura(NodoArbol* nodo);
    void mostrarArbolPiramide(NodoArbol* nodo);
    void cargarDatosPorDefecto();
    NodoArbol* buscarNodoRec(NodoArbol* nodo, int id);
    NodoArbol* buscarPrimerSucesorValido(NodoArbol* subraiz, bool incluirEnCarcel = false);

public:
    ArbolMafia();
    ~ArbolMafia();

    void cargarDesdeCSV(const string& rutaArchivo);
    void insertarEnArbol(Persona p);
    void mostrarLineaSucesion();
    void verificarYActualizarJefe();
    void editarMiembro(int id);
    NodoArbol* getRaiz() { return raiz; }
};

#endif