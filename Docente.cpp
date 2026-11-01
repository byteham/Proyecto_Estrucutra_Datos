#include "Docente.h"

Docente::Docente(const std::string& c, const std::string& n, const std::string& a)
    : cedula(c), nombres(n), apellidos(a) {}

const std::string& Docente::getCedula() const { return cedula; }
const std::string& Docente::getNombres() const { return nombres; }
const std::string& Docente::getApellidos() const { return apellidos; }

std::string Docente::nombreCompleto() const {
    return nombres + " " + apellidos;
}
