#pragma once
#include <string>

// Un momento en el tiempo: fecha "dd/mm/aaaa", minutos desde la medianoche y dia (0 = domingo ... 6 = sabado).
struct Instante {
    std::string fecha;
    int minutos;
    int diaSemana;
    Instante() : minutos(0), diaSemana(0) {}
};

namespace Utilidades {

std::string limpiar(const std::string& texto);                  // quita espacios al inicio y al final

// --- Reloj ---
Instante obtenerInstanteActual();
bool construirInstante(const std::string& fecha, int minutos, Instante& salida);  // false si la fecha no existe

// --- Entrada por teclado: todas repiten la pregunta hasta recibir un dato valido ---
std::string leerTexto(const std::string& mensaje);
int leerOpcion(const std::string& mensaje, int minimo, int maximo);
std::string leerCedula(const std::string& mensaje);
std::string leerNrc(const std::string& mensaje);
std::string leerLetras(const std::string& mensaje, const std::string& etiqueta);
int leerHora(const std::string& mensaje);                       // devuelve minutos desde la medianoche
void leerFecha(const std::string& mensaje, std::string& fecha, int minutos, Instante& instante);
void pausar();

}  // namespace Utilidades
