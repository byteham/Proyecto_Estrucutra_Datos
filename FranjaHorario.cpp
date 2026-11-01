#include "FranjaHorario.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

FranjaHorario::FranjaHorario() : numero(1), horaInicio(7), horaFin(9) {}

FranjaHorario::FranjaHorario(int n) {
    if (!esNumeroValido(n)) {
        throw std::invalid_argument("Numero de franja invalido.");
    }
    numero = n;
    horaInicio = 5 + 2 * n;
    horaFin = horaInicio + 2;
}

int FranjaHorario::getNumero() const { return numero; }
int FranjaHorario::getHoraInicio() const { return horaInicio; }
int FranjaHorario::getHoraFin() const { return horaFin; }
int FranjaHorario::inicioEnMinutos() const { return horaInicio * 60; }
int FranjaHorario::finEnMinutos() const { return horaFin * 60; }
int FranjaHorario::limiteIngresoPuntual() const { return inicioEnMinutos() + MINUTOS_PUNTUAL; }
int FranjaHorario::inicioVentanaSalida() const { return finEnMinutos() - MINUTOS_SALIDA; }

bool FranjaHorario::contiene(int minutos) const {
    return minutos >= inicioEnMinutos() && minutos < finEnMinutos();
}

std::string FranjaHorario::aTexto() const {
    return formatoHora(inicioEnMinutos()) + "-" + formatoHora(finEnMinutos());
}

bool FranjaHorario::operator==(const FranjaHorario& otra) const {
    return numero == otra.numero;
}

bool FranjaHorario::esNumeroValido(int n) {
    return n >= 1 && n <= TOTAL;
}

int FranjaHorario::numeroParaMinuto(int minutos) {
    for (int i = 1; i <= TOTAL; ++i) {
        if (FranjaHorario(i).contiene(minutos)) return i;
    }
    return 0;
}

std::string FranjaHorario::formatoHora(int minutos) {
    std::ostringstream salida;
    salida << std::setfill('0') << std::setw(2) << (minutos / 60)
           << ':' << std::setw(2) << (minutos % 60);
    return salida.str();
}
