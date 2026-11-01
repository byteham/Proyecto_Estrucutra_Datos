#pragma once
#include <string>

// Franja de 2 horas: 1 = 07-09, 2 = 09-11, ... 7 = 19-21.
class FranjaHorario {
public:
    static const int TOTAL = 7;
    static const int MINUTOS_PUNTUAL = 10;  // ingreso puntual hasta inicio + 10 min
    static const int MINUTOS_SALIDA = 10;   // la salida se registra en los ultimos 10 min

    FranjaHorario();                        // franja 1
    explicit FranjaHorario(int numero);     // lanza std::invalid_argument si no esta entre 1 y 7

    int getNumero() const;
    int getHoraInicio() const;
    int getHoraFin() const;
    int inicioEnMinutos() const;
    int finEnMinutos() const;
    int limiteIngresoPuntual() const;       // ej. 07:10 -> 430
    int inicioVentanaSalida() const;        // ej. 08:50 -> 530
    bool contiene(int minutos) const;
    std::string aTexto() const;             // "07:00-09:00"
    bool operator==(const FranjaHorario& otra) const;

    static bool esNumeroValido(int numero);
    static int numeroParaMinuto(int minutos);       // 0 si no hay franja a esa hora
    static std::string formatoHora(int minutos);    // 425 -> "07:05"

private:
    int numero;
    int horaInicio;
    int horaFin;
};
