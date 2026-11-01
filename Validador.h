#pragma once
#include <string>

// Todas las validaciones del sistema. Son funciones estaticas: no leen ni imprimen nada,
// solo reciben un texto y responden. Asi se pueden probar con pruebas unitarias.
class Validador {
public:
    static bool soloNumeros(const std::string& texto);
    static bool soloLetras(const std::string& texto);       // letras A-Z y espacios simples entre palabras

    static bool validarCedula(const std::string& cedula);
    static std::string motivoCedulaInvalida(const std::string& cedula);  // "" si es valida

    static bool validarNrc(const std::string& nrc);
    static std::string motivoNrcInvalido(const std::string& nrc);        // "" si es valido

    static bool validarHora(const std::string& hora);       // formato HH:MM de 00:00 a 23:59
    static int horaAMinutos(const std::string& hora);       // "07:10" -> 430 (-1 si es invalida)
    static bool validarFecha(const std::string& fecha);     // formato dd/mm/aaaa
};
