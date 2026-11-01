#pragma once

// Nodo de una lista doblemente enlazada (plantilla: sirve para cualquier tipo T).
template <typename T>
class ListaDoble;

template <typename T>
class Nodo {
    friend class ListaDoble<T>;

private:
    T dato;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

public:
    explicit Nodo(const T& valor) : dato(valor), siguiente(nullptr), anterior(nullptr) {}
};
