#pragma once
#include <string>

enum class EstadoAsistencia { PUNTUAL, ATRASO, AUSENTE };

inline std::string estadoATexto(EstadoAsistencia estado) {
    switch (estado) {
        case EstadoAsistencia::PUNTUAL: return "PUNTUAL";
        case EstadoAsistencia::ATRASO:  return "ATRASO";
        case EstadoAsistencia::AUSENTE: return "AUSENTE";
    }
    return "DESCONOCIDO";
}
