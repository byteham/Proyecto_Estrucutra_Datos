#include "Curso.h"

Curso::Curso(const std::string& n, const std::string& m)
    : nrc(n), materia(m), cedulaDocente("") {}

const std::string& Curso::getNrc() const { return nrc; }
const std::string& Curso::getMateria() const { return materia; }
const std::string& Curso::getCedulaDocente() const { return cedulaDocente; }

bool Curso::tieneDocente() const { return !cedulaDocente.empty(); }

void Curso::asignarDocente(const std::string& cedula) { cedulaDocente = cedula; }

bool Curso::agregarHorario(DiaSemana dia, const FranjaHorario& franja) {
    if (tieneCruce(dia, franja)) return false;
    horarios.insertarFinal(HorarioCurso(nrc, dia, franja));
    return true;
}

bool Curso::tieneCruce(DiaSemana dia, const FranjaHorario& franja) const {
    return horarios.buscar([&](const HorarioCurso& h) {
               return h.coincideCon(dia, franja);
           }) != nullptr;
}

const ListaDoble<HorarioCurso>& Curso::getHorarios() const { return horarios; }
