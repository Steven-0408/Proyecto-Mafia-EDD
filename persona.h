#ifndef PERSONA_H
#define PERSONA_H
#include <string>
using namespace std;

struct Persona {
    int id;
    string name;
    string last_name;
    char gender; // 'H' o 'M'
    int age;
    int id_boss;
    int is_dead;  // 0 o 1
    int in_jail;  // 0 o 1
    int was_boss; // 0 o 1
    int is_boss;  // 0 o 1
};

struct NodoArbol {
    Persona persona;
    NodoArbol* izq;
    NodoArbol* der;

    NodoArbol(Persona p) : persona(p), izq(nullptr), der(nullptr) {}
};

#endif