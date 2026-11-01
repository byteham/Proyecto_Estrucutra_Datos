#include "RegistroAsistencia.h"

RegistroAsistencia::RegistroAsistencia(const std::string& f, const std::string& n,
                                       const std::string& c, const FranjaHorario& fr)
    : fecha(f), nrc(n), cedulaDocente(c), franja(fr),
      horaIngreso(-1), horaSalida(-1), estado(EstadoAsistencia::AUSENTE) {}

void RegistroAsistencia::registrarIngreso(int minutos) {
    horaIngreso = minutos;
    estado = (minutos <= franja.limiteIngresoPuntual()) ? EstadoAsistencia::PUNTUAL
                                                         : EstadoAsistencia::ATRASO;
}

void RegistroAsistencia::registrarSalida(int minutos) { horaSalida = minutos; }

void RegistroAsistencia::marcarAusente() {
    horaIngreso = -1;
    horaSalida = -1;
    estado = EstadoAsistencia::AUSENTE;
}

bool RegistroAsistencia::tieneIngreso() const { return horaIngreso >= 0; }
bool RegistroAsistencia::tieneSalida() const { return horaSalida >= 0; }

const std::string& RegistroAsistencia::getFecha() const { return fecha; }
const std::string& RegistroAsistencia::getNrc() const { return nrc; }
const std::string& RegistroAsistencia::getCedulaDocente() const { return cedulaDocente; }
const FranjaHorario& RegistroAsistencia::getFranja() const { return franja; }
int RegistroAsistencia::getHoraIngreso() const { return horaIngreso; }
int RegistroAsistencia::getHoraSalida() const { return horaSalida; }
EstadoAsistencia RegistroAsistencia::getEstado() const { return estado; }
