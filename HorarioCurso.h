#pragma once
#include <string>
#include "DiaSemana.h"
#include "FranjaHorario.h"

// Una sesion semanal de un NRC: un dia y una franja.
class HorarioCurso {
public:
    HorarioCurso(const std::string& nrc, DiaSemana dia, const FranjaHorario& franja);

    const std::string& getNrc() const;
    DiaSemana getDia() const;
    const FranjaHorario& getFranja() const;
    bool coincideCon(DiaSemana dia, const FranjaHorario& franja) const;

private:
    std::string nrc;
    DiaSemana dia;
    FranjaHorario franja;
};
