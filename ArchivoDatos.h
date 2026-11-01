#pragma once
#include <string>
#include <vector>
#include "Sistema.h"

// Resultado de leer el archivo de texto.
struct ResumenCarga {
    bool archivoExiste;
    int cargados;                       // lineas aceptadas
    int ignorados;                      // lineas rechazadas por estar danadas o ser invalidas
    std::vector<std::string> avisos;    // las primeras lineas rechazadas, con el motivo
    ResumenCarga() : archivoExiste(false), cargados(0), ignorados(0) {}
};

// Guarda y carga todo el sistema en un archivo .txt.
//
// Formato (una linea por dato, campos separados por '|'):
//   D|cedula|nombres|apellidos                                  docente
//   C|nrc|materia|cedulaDocente  (o '-' si no tiene)            curso
//   H|nrc|dia(1-6)|franja(1-7)                                  horario de un curso
//   R|fecha|nrc|cedula|franja|ingreso|salida|estado             asistencia (horas en minutos, '-' si no hay)
//
// Al cargar, cada linea pasa por las mismas validaciones que el menu: una linea invalida se
// ignora y se avisa, pero el programa nunca se cae.
class ArchivoDatos {
public:
    explicit ArchivoDatos(const std::string& ruta);

    const std::string& getRuta() const;
    bool guardar(const Sistema& sistema, std::string& error) const;
    ResumenCarga cargar(Sistema& sistema) const;

private:
    std::string ruta;
};
