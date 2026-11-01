#pragma once
#include "ArchivoDatos.h"
#include "Sistema.h"
#include "Utilidades.h"

// Interfaz de consola. Solo pide datos, llama a Sistema y muestra los resultados.
class Menu {
public:
    explicit Menu(const std::string& rutaArchivo = "datos_asistencia.txt");
    void ejecutar();

private:
    Sistema sistema;
    ArchivoDatos archivo;
    bool horaSimulada;
    Instante simulado;

    Instante instante() const;
    void mostrarResultado(const Resultado& resultado);   // si salio bien, guarda en el archivo
    void cargarDatos();
    void guardarCambios();
    void mostrarEncabezado(const std::string& titulo) const;

    // Menu principal
    void registrarIngreso();
    void registrarSalida();
    void simularFechaHora();

    // Menu de administrador
    void menuAdministrador();
    void registrarDocente();
    void crearCurso();
    void asignarDocente();
    void agregarHorario();
    void listarDocentes() const;
    void reporteAsistencia() const;
    void marcarAusentes();
    void cargarDatosEjemplo();
};
