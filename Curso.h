#pragma once
#include <string>
#include "DiaSemana.h"
#include "FranjaHorario.h"
#include "HorarioCurso.h"
#include "ListaDoble.h"

class Curso {
public:
    Curso(const std::string& nrc, const std::string& materia);

    const std::string& getNrc() const;
    const std::string& getMateria() const;
    const std::string& getCedulaDocente() const;

    bool tieneDocente() const;
    void asignarDocente(const std::string& cedula);

    // Agrega un dia/franja al curso. Devuelve false si el curso ya lo tenia.
    bool agregarHorario(DiaSemana dia, const FranjaHorario& franja);

    // true si el curso ya tiene clase en ese dia y franja.
    bool tieneCruce(DiaSemana dia, const FranjaHorario& franja) const;

    const ListaDoble<HorarioCurso>& getHorarios() const;

private:
    std::string nrc;
    std::string materia;
    std::string cedulaDocente;
    ListaDoble<HorarioCurso> horarios;
};
