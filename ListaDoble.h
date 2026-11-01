#pragma once
#include <cstddef>
#include <functional>
#include <utility>
#include "Nodo.h"

// Lista doblemente enlazada generica.
// Maneja su propia memoria: copia profunda, destructor y operador de asignacion.
template <typename T>
class ListaDoble {
public:
    typedef std::function<bool(const T&)> Criterio;
    typedef std::function<void(const T&)> Accion;

    ListaDoble() : cabeza(nullptr), cola(nullptr), tamanio(0) {}

    ListaDoble(const ListaDoble& otra) : cabeza(nullptr), cola(nullptr), tamanio(0) {
        for (Nodo<T>* n = otra.cabeza; n != nullptr; n = n->siguiente) {
            insertarFinal(n->dato);
        }
    }

    ListaDoble& operator=(ListaDoble otra) {
        intercambiar(otra);
        return *this;
    }

    ~ListaDoble() { vaciar(); }

    void insertarFinal(const T& dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (cola == nullptr) {
            cabeza = cola = nuevo;
        } else {
            nuevo->anterior = cola;
            cola->siguiente = nuevo;
            cola = nuevo;
        }
        ++tamanio;
    }

    void insertarInicio(const T& dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (cabeza == nullptr) {
            cabeza = cola = nuevo;
        } else {
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
            cabeza = nuevo;
        }
        ++tamanio;
    }

    // Devuelve un puntero al primer elemento que cumpla el criterio, o nullptr si no existe.
    T* buscar(const Criterio& criterio) {
        for (Nodo<T>* n = cabeza; n != nullptr; n = n->siguiente) {
            if (criterio(n->dato)) return &n->dato;
        }
        return nullptr;
    }

    const T* buscar(const Criterio& criterio) const {
        for (const Nodo<T>* n = cabeza; n != nullptr; n = n->siguiente) {
            if (criterio(n->dato)) return &n->dato;
        }
        return nullptr;
    }

    // Elimina el primer elemento que cumpla el criterio. Devuelve false si no hubo ninguno.
    bool eliminar(const Criterio& criterio) {
        for (Nodo<T>* n = cabeza; n != nullptr; n = n->siguiente) {
            if (criterio(n->dato)) {
                if (n->anterior != nullptr) n->anterior->siguiente = n->siguiente;
                else cabeza = n->siguiente;
                if (n->siguiente != nullptr) n->siguiente->anterior = n->anterior;
                else cola = n->anterior;
                delete n;
                --tamanio;
                return true;
            }
        }
        return false;
    }

    // Recorre de la cabeza hacia la cola.
    void recorrerAdelante(const Accion& accion) const {
        for (const Nodo<T>* n = cabeza; n != nullptr; n = n->siguiente) accion(n->dato);
    }

    // Recorre de la cola hacia la cabeza.
    void recorrerAtras(const Accion& accion) const {
        for (const Nodo<T>* n = cola; n != nullptr; n = n->anterior) accion(n->dato);
    }

    std::size_t obtenerTamanio() const { return tamanio; }
    bool estaVacia() const { return tamanio == 0; }

    void vaciar() {
        while (cabeza != nullptr) {
            Nodo<T>* siguiente = cabeza->siguiente;
            delete cabeza;
            cabeza = siguiente;
        }
        cola = nullptr;
        tamanio = 0;
    }

private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    std::size_t tamanio;

    void intercambiar(ListaDoble& otra) {
        std::swap(cabeza, otra.cabeza);
        std::swap(cola, otra.cola);
        std::swap(tamanio, otra.tamanio);
    }
};
