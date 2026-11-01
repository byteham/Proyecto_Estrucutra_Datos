#include "HorarioCurso.h"

HorarioCurso::HorarioCurso(const std::string& n, DiaSemana d, const FranjaHorario& f)
    : nrc(n), dia(d), franja(f) {}

const std::string& HorarioCurso::getNrc() const { return nrc; }
DiaSemana HorarioCurso::getDia() const { return dia; }
const FranjaHorario& HorarioCurso::getFranja() const { return franja; }

bool HorarioCurso::coincideCon(DiaSemana d, const FranjaHorario& f) const {
    return dia == d && franja == f;
}
