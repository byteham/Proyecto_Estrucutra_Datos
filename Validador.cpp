#include "Validador.h"
#include <regex>

bool Validador::soloNumeros(const std::string& texto) {
    static const std::regex patron("^[0-9]+$");
    return std::regex_match(texto, patron);
}

bool Validador::soloLetras(const std::string& texto) {
    static const std::regex patron("^[A-Za-z]+( [A-Za-z]+)*$");
    return std::regex_match(texto, patron);
}

std::string Validador::motivoCedulaInvalida(const std::string& cedula) {
    if (cedula.empty()) return "La cedula no puede estar vacia.";
    if (!soloNumeros(cedula)) return "La cedula solo debe contener numeros.";
    if (cedula.size() != 10) return "La cedula debe tener exactamente 10 digitos.";

    int provincia = (cedula[0] - '0') * 10 + (cedula[1] - '0');
    if (provincia < 1 || (provincia > 24 && provincia != 30)) {
        return "Los dos primeros digitos (provincia) no son validos.";
    }
    if (cedula[2] - '0' > 5) {
        return "El tercer digito no es valido para una cedula de persona natural.";
    }

    // Algoritmo modulo 10: posiciones impares x2 (si pasa de 9 se resta 9)
    int suma = 0;
    for (int i = 0; i < 9; ++i) {
        int d = cedula[i] - '0';
        if (i % 2 == 0) {
            d *= 2;
            if (d > 9) d -= 9;
        }
        suma += d;
    }
    int verificador = (10 - suma % 10) % 10;
    if (verificador != cedula[9] - '0') {
        return "La cedula no es valida (el digito verificador no coincide).";
    }
    return "";
}

bool Validador::validarCedula(const std::string& cedula) {
    return motivoCedulaInvalida(cedula).empty();
}

std::string Validador::motivoNrcInvalido(const std::string& nrc) {
    if (nrc.empty()) return "El NRC no puede estar vacio.";
    if (!soloNumeros(nrc)) return "El NRC solo debe contener numeros.";
    if (nrc.size() < 4 || nrc.size() > 6) return "El NRC debe tener entre 4 y 6 digitos.";
    return "";
}

bool Validador::validarNrc(const std::string& nrc) {
    return motivoNrcInvalido(nrc).empty();
}

bool Validador::validarHora(const std::string& hora) {
    static const std::regex patron("^([01][0-9]|2[0-3]):[0-5][0-9]$");
    return std::regex_match(hora, patron);
}

int Validador::horaAMinutos(const std::string& hora) {
    if (!validarHora(hora)) return -1;
    return std::stoi(hora.substr(0, 2)) * 60 + std::stoi(hora.substr(3, 2));
}

bool Validador::validarFecha(const std::string& fecha) {
    static const std::regex patron("^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/[0-9]{4}$");
    return std::regex_match(fecha, patron);
}
