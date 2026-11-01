#include "ArchivoDatos.h"
#include <cstdio>
#include <fstream>
#include "Utilidades.h"
#include "Validador.h"

namespace {

std::vector<std::string> dividir(const std::string& linea, char separador) {
    std::vector<std::string> partes;
    std::string actual;
    for (std::size_t i = 0; i < linea.size(); ++i) {
        if (linea[i] == separador) {
            partes.push_back(actual);
            actual.clear();
        } else {
            actual += linea[i];
        }
    }
    partes.push_back(actual);
    return partes;
}

bool leerEntero(const std::string& texto, int& valor) {
    if (!Validador::soloNumeros(texto) || texto.size() > 6) return false;
    valor = std::stoi(texto);
    return true;
}

// "-" significa "sin registrar" (-1).
bool leerMinutos(const std::string& texto, int& valor) {
    if (texto == "-") {
        valor = -1;
        return true;
    }
    return leerEntero(texto, valor) && valor <= 1439;
}

std::string minutosATexto(int minutos) {
    return minutos < 0 ? "-" : std::to_string(minutos);
}

// Devuelve "" si la linea se cargo bien, o el motivo por el que se rechazo.
std::string procesarLinea(Sistema& sistema, const std::string& linea) {
    std::vector<std::string> p = dividir(linea, '|');
    const std::string& tipo = p[0];

    if (tipo == "D") {
        if (p.size() != 4) return "formato de docente incorrecto";
        Resultado r = sistema.registrarDocente(p[1], p[2], p[3]);
        return r.exito ? "" : r.mensaje;
    }

    if (tipo == "C") {
        if (p.size() != 4) return "formato de curso incorrecto";
        Resultado r = sistema.crearCurso(p[1], p[2]);
        if (!r.exito) return r.mensaje;
        if (p[3] != "-") {
            Resultado a = sistema.asignarDocente(p[1], p[3]);
            if (!a.exito) return a.mensaje;
        }
        return "";
    }

    if (tipo == "H") {
        if (p.size() != 4) return "formato de horario incorrecto";
        int numeroDia = 0, franja = 0;
        DiaSemana dia = DiaSemana::LUNES;
        if (!leerEntero(p[2], numeroDia) || !diaDesdeNumero(numeroDia, dia)) return "dia invalido";
        if (!leerEntero(p[3], franja)) return "franja invalida";
        Resultado r = sistema.agregarHorario(p[1], dia, franja);
        return r.exito ? "" : r.mensaje;
    }

    if (tipo == "R") {
        if (p.size() != 8) return "formato de registro incorrecto";
        if (!Validador::validarFecha(p[1])) return "fecha invalida";
        if (!Validador::validarNrc(p[2])) return "NRC invalido";
        if (!Validador::validarCedula(p[3])) return "cedula invalida";
        int numeroFranja = 0, ingreso = -1, salida = -1;
        if (!leerEntero(p[4], numeroFranja) || !FranjaHorario::esNumeroValido(numeroFranja))
            return "franja invalida";
        if (!leerMinutos(p[5], ingreso) || !leerMinutos(p[6], salida)) return "hora invalida";

        RegistroAsistencia registro(p[1], p[2], p[3], FranjaHorario(numeroFranja));
        if (ingreso >= 0) registro.registrarIngreso(ingreso);
        else registro.marcarAusente();
        if (salida >= 0) {
            if (ingreso < 0) return "tiene salida pero no ingreso";
            registro.registrarSalida(salida);
        }
        if (estadoATexto(registro.getEstado()) != p[7]) return "el estado no coincide con las horas";

        Resultado r = sistema.cargarRegistro(registro);
        return r.exito ? "" : r.mensaje;
    }

    return "tipo de linea desconocido";
}

}  // namespace

ArchivoDatos::ArchivoDatos(const std::string& r) : ruta(r) {}

const std::string& ArchivoDatos::getRuta() const { return ruta; }

bool ArchivoDatos::guardar(const Sistema& sistema, std::string& error) const {
    // Se escribe primero en un archivo temporal y luego se reemplaza el original,
    // para no dejar el archivo a medias si algo falla.
    const std::string temporal = ruta + ".tmp";
    {
        std::ofstream salida(temporal.c_str());
        if (!salida.is_open()) {
            error = "No se pudo crear el archivo " + temporal;
            return false;
        }
        salida << "# Sistema de asistencia docente - archivo generado por el programa\n";
        salida << "VERSION|1\n";

        sistema.getDocentes().recorrerAdelante([&](const Docente& d) {
            salida << "D|" << d.getCedula() << '|' << d.getNombres() << '|' << d.getApellidos() << '\n';
        });
        sistema.getCursos().recorrerAdelante([&](const Curso& c) {
            salida << "C|" << c.getNrc() << '|' << c.getMateria() << '|'
                   << (c.tieneDocente() ? c.getCedulaDocente() : std::string("-")) << '\n';
        });
        sistema.getCursos().recorrerAdelante([&](const Curso& c) {
            c.getHorarios().recorrerAdelante([&](const HorarioCurso& h) {
                salida << "H|" << c.getNrc() << '|' << static_cast<int>(h.getDia()) << '|'
                       << h.getFranja().getNumero() << '\n';
            });
        });
        sistema.getRegistros().recorrerAdelante([&](const RegistroAsistencia& r) {
            salida << "R|" << r.getFecha() << '|' << r.getNrc() << '|' << r.getCedulaDocente() << '|'
                   << r.getFranja().getNumero() << '|' << minutosATexto(r.getHoraIngreso()) << '|'
                   << minutosATexto(r.getHoraSalida()) << '|' << estadoATexto(r.getEstado()) << '\n';
        });

        salida.flush();
        if (!salida.good()) {
            error = "Error al escribir en " + temporal + " (disco lleno o sin permisos).";
            return false;
        }
    }
    std::remove(ruta.c_str());
    if (std::rename(temporal.c_str(), ruta.c_str()) != 0) {
        error = "No se pudo reemplazar el archivo " + ruta;
        return false;
    }
    return true;
}

ResumenCarga ArchivoDatos::cargar(Sistema& sistema) const {
    ResumenCarga resumen;
    std::ifstream entrada(ruta.c_str());
    if (!entrada.is_open()) return resumen;   // primera ejecucion: aun no existe
    resumen.archivoExiste = true;

    std::string linea;
    int numero = 0;
    while (std::getline(entrada, linea)) {
        ++numero;
        linea = Utilidades::limpiar(linea);
        if (linea.empty() || linea[0] == '#' || linea.compare(0, 8, "VERSION|") == 0) continue;

        std::string motivo = procesarLinea(sistema, linea);
        if (motivo.empty()) {
            ++resumen.cargados;
        } else {
            ++resumen.ignorados;
            if (resumen.avisos.size() < 5) {
                resumen.avisos.push_back("Linea " + std::to_string(numero) + " ignorada: " + motivo);
            }
        }
    }
    return resumen;
}
