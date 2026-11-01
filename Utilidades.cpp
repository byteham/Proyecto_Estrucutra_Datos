#include "Utilidades.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Validador.h"

namespace Utilidades {

std::string limpiar(const std::string& texto) {
    const std::string espacios = " \t\r\n";
    std::size_t inicio = texto.find_first_not_of(espacios);
    if (inicio == std::string::npos) return "";
    std::size_t fin = texto.find_last_not_of(espacios);
    return texto.substr(inicio, fin - inicio + 1);
}

// ------------------------------------------------------------------ reloj

Instante obtenerInstanteActual() {
    std::time_t ahora = std::time(nullptr);
    std::tm local = std::tm();
#ifdef _WIN32
    localtime_s(&local, &ahora);
#else
    localtime_r(&ahora, &local);
#endif
    std::ostringstream fecha;
    fecha << std::setfill('0') << std::setw(2) << local.tm_mday << '/'
          << std::setw(2) << (local.tm_mon + 1) << '/' << (local.tm_year + 1900);

    Instante instante;
    instante.fecha = fecha.str();
    instante.minutos = local.tm_hour * 60 + local.tm_min;
    instante.diaSemana = local.tm_wday;
    return instante;
}

bool construirInstante(const std::string& fecha, int minutos, Instante& salida) {
    if (!Validador::validarFecha(fecha)) return false;
    std::tm base = std::tm();
    base.tm_mday = std::stoi(fecha.substr(0, 2));
    base.tm_mon = std::stoi(fecha.substr(3, 2)) - 1;
    base.tm_year = std::stoi(fecha.substr(6, 4)) - 1900;
    base.tm_hour = 12;
    std::tm copia = base;
    if (std::mktime(&copia) == static_cast<std::time_t>(-1)) return false;
    // Si el dia o el mes cambiaron, la fecha no existia (por ejemplo 31/02).
    if (copia.tm_mday != base.tm_mday || copia.tm_mon != base.tm_mon) return false;

    salida.fecha = fecha;
    salida.minutos = minutos;
    salida.diaSemana = copia.tm_wday;
    return true;
}

// ------------------------------------------------------------------ teclado

std::string leerTexto(const std::string& mensaje) {
    std::cout << mensaje;
    std::string linea;
    if (!std::getline(std::cin, linea)) {
        std::cout << "\nEntrada finalizada. Cerrando el programa.\n";
        std::exit(0);
    }
    return limpiar(linea);
}

int leerOpcion(const std::string& mensaje, int minimo, int maximo) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        if (!Validador::soloNumeros(texto)) {
            std::cout << "  Error: ingrese solo numeros.\n";
            continue;
        }
        if (texto.size() > 4) {
            std::cout << "  Error: el valor es demasiado largo.\n";
            continue;
        }
        int valor = std::stoi(texto);
        if (valor < minimo || valor > maximo) {
            std::cout << "  Error: elija un valor entre " << minimo << " y " << maximo << ".\n";
            continue;
        }
        return valor;
    }
}

std::string leerCedula(const std::string& mensaje) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        std::string motivo = Validador::motivoCedulaInvalida(texto);
        if (motivo.empty()) return texto;
        std::cout << "  Error: " << motivo << "\n";
    }
}

std::string leerNrc(const std::string& mensaje) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        std::string motivo = Validador::motivoNrcInvalido(texto);
        if (motivo.empty()) return texto;
        std::cout << "  Error: " << motivo << "\n";
    }
}

std::string leerLetras(const std::string& mensaje, const std::string& etiqueta) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        if (Validador::soloLetras(texto)) return texto;
        std::cout << "  Error: " << etiqueta
                  << " solo debe contener letras (sin numeros ni simbolos).\n";
    }
}

int leerHora(const std::string& mensaje) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        int minutos = Validador::horaAMinutos(texto);
        if (minutos >= 0) return minutos;
        std::cout << "  Error: use el formato HH:MM de 24 horas, por ejemplo 07:05.\n";
    }
}

void leerFecha(const std::string& mensaje, std::string& fecha, int minutos, Instante& instante) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        if (construirInstante(texto, minutos, instante)) {
            fecha = texto;
            return;
        }
        std::cout << "  Error: fecha invalida. Use dd/mm/aaaa con una fecha que exista.\n";
    }
}

void pausar() {
    leerTexto("\nPresione ENTER para continuar...");
}

}  // namespace Utilidades
