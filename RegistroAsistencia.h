#pragma once
#include <string>
#include "EstadoAsistencia.h"
#include "FranjaHorario.h"

// Asistencia de un docente a un NRC en una fecha y franja concretas.
// Las horas se guardan en minutos desde la medianoche (07:05 -> 425). -1 significa "sin registrar".
class RegistroAsistencia {
public:
    RegistroAsistencia(const std::string& fecha, const std::string& nrc,
                       const std::string& cedulaDocente, const FranjaHorario& franja);

    void registrarIngreso(int minutos);   // define PUNTUAL o ATRASO segun la hora
    void registrarSalida(int minutos);
    void marcarAusente();

    bool tieneIngreso() const;
    bool tieneSalida() const;

    const std::string& getFecha() const;
    const std::string& getNrc() const;
    const std::string& getCedulaDocente() const;
    const FranjaHorario& getFranja() const;
    int getHoraIngreso() const;
    int getHoraSalida() const;
    EstadoAsistencia getEstado() const;

private:
    std::string fecha;
    std::string nrc;
    std::string cedulaDocente;
    FranjaHorario franja;
    int horaIngreso;
    int horaSalida;
    EstadoAsistencia estado;
};
