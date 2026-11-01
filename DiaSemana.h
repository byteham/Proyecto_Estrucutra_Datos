#pragma once
#include <string>

// Dias en los que hay clases (el numero coincide con tm_wday: 1 = lunes ... 6 = sabado).
enum class DiaSemana { LUNES = 1, MARTES, MIERCOLES, JUEVES, VIERNES, SABADO };

inline std::string nombreDia(DiaSemana dia) {
    switch (dia) {
        case DiaSemana::LUNES:     return "Lunes";
        case DiaSemana::MARTES:    return "Martes";
        case DiaSemana::MIERCOLES: return "Miercoles";
        case DiaSemana::JUEVES:    return "Jueves";
        case DiaSemana::VIERNES:   return "Viernes";
        case DiaSemana::SABADO:    return "Sabado";
    }
    return "Desconocido";
}

// Convierte 1..6 en DiaSemana. Devuelve false para cualquier otro valor (por ejemplo domingo = 0).
inline bool diaDesdeNumero(int numero, DiaSemana& dia) {
    if (numero < 1 || numero > 6) return false;
    dia = static_cast<DiaSemana>(numero);
    return true;
}
