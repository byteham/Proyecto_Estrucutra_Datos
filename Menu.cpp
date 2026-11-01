#include "Menu.h"
#include <iomanip>
#include <iostream>
#include "Validador.h"

using std::cout;

Menu::Menu(const std::string& ruta) : archivo(ruta), horaSimulada(false) {}

Instante Menu::instante() const {
    return horaSimulada ? simulado : Utilidades::obtenerInstanteActual();
}

void Menu::mostrarResultado(const Resultado& resultado) {
    cout << (resultado.exito ? "\n  [OK] " : "\n  [ERROR] ") << resultado.mensaje << "\n";
    if (resultado.exito) guardarCambios();
}

void Menu::cargarDatos() {
    ResumenCarga resumen = archivo.cargar(sistema);
    if (!resumen.archivoExiste) {
        cout << "\n  No existe " << archivo.getRuta()
             << ": se creara al guardar el primer cambio.\n";
        return;
    }
    cout << "\n  Datos cargados desde " << archivo.getRuta() << ": "
         << sistema.getDocentes().obtenerTamanio() << " docente(s), "
         << sistema.getCursos().obtenerTamanio() << " curso(s), "
         << sistema.getRegistros().obtenerTamanio() << " registro(s).\n";
    if (resumen.ignorados > 0) {
        cout << "  Aviso: " << resumen.ignorados << " linea(s) del archivo se ignoraron:\n";
        for (std::size_t i = 0; i < resumen.avisos.size(); ++i) cout << "    - " << resumen.avisos[i] << "\n";
    }
}

void Menu::guardarCambios() {
    std::string error;
    if (archivo.guardar(sistema, error)) {
        cout << "  Datos guardados en " << archivo.getRuta() << ".\n";
    } else {
        cout << "  [AVISO] No se pudieron guardar los datos: " << error << "\n";
    }
}

void Menu::mostrarEncabezado(const std::string& titulo) const {
    cout << "\n==========================================\n";
    cout << "  " << titulo << "\n";
    cout << "==========================================\n";
}

void Menu::ejecutar() {
    cargarDatos();
    while (true) {
        Instante ahora = instante();
        DiaSemana dia;
        std::string textoDia = diaDesdeNumero(ahora.diaSemana, dia) ? nombreDia(dia) : "Domingo";

        mostrarEncabezado("CONTROL DE ASISTENCIA DOCENTE - ESPE");
        cout << "  Fecha y hora: " << textoDia << " " << ahora.fecha << " "
             << FranjaHorario::formatoHora(ahora.minutos)
             << (horaSimulada ? "  (SIMULADA)" : "") << "\n\n";
        cout << "  [1] Registrar ingreso\n";
        cout << "  [2] Registrar salida\n";
        cout << "  [3] Menu de administrador\n";
        cout << "  [4] Simular fecha y hora (para pruebas)\n";
        cout << "  [0] Salir\n\n";

        int opcion = Utilidades::leerOpcion("  Seleccione una opcion: ", 0, 4);
        switch (opcion) {
            case 1: registrarIngreso(); break;
            case 2: registrarSalida(); break;
            case 3: menuAdministrador(); break;
            case 4: simularFechaHora(); break;
            case 0: cout << "\nHasta luego.\n"; return;
        }
    }
}

// ---------------------------------------------------------------- docente

void Menu::registrarIngreso() {
    mostrarEncabezado("REGISTRAR INGRESO");
    Instante ahora = instante();
    DiaSemana dia;
    if (!diaDesdeNumero(ahora.diaSemana, dia)) {
        cout << "\n  [ERROR] Hoy es domingo: no hay clases.\n";
        Utilidades::pausar();
        return;
    }
    std::string cedula = Utilidades::leerCedula("  Ingrese su cedula: ");
    mostrarResultado(sistema.registrarIngreso(cedula, dia, ahora.minutos, ahora.fecha));
    Utilidades::pausar();
}

void Menu::registrarSalida() {
    mostrarEncabezado("REGISTRAR SALIDA");
    Instante ahora = instante();
    DiaSemana dia;
    if (!diaDesdeNumero(ahora.diaSemana, dia)) {
        cout << "\n  [ERROR] Hoy es domingo: no hay clases.\n";
        Utilidades::pausar();
        return;
    }
    std::string cedula = Utilidades::leerCedula("  Ingrese su cedula: ");
    mostrarResultado(sistema.registrarSalida(cedula, dia, ahora.minutos, ahora.fecha));
    Utilidades::pausar();
}

void Menu::simularFechaHora() {
    mostrarEncabezado("SIMULAR FECHA Y HORA");
    cout << "  [1] Usar una fecha y hora de prueba\n";
    cout << "  [2] Volver a la hora real\n";
    cout << "  [0] Cancelar\n\n";
    int opcion = Utilidades::leerOpcion("  Seleccione una opcion: ", 0, 2);
    if (opcion == 0) return;
    if (opcion == 2) {
        horaSimulada = false;
        cout << "\n  Se usa la hora real del equipo.\n";
        Utilidades::pausar();
        return;
    }
    int minutos = Utilidades::leerHora("  Hora (HH:MM, 24 horas): ");
    std::string fecha;
    Instante nuevo;
    Utilidades::leerFecha("  Fecha (dd/mm/aaaa): ", fecha, minutos, nuevo);
    simulado = nuevo;
    horaSimulada = true;
    cout << "\n  Fecha y hora simuladas configuradas.\n";
    Utilidades::pausar();
}

// ---------------------------------------------------------------- administrador

void Menu::menuAdministrador() {
    while (true) {
        mostrarEncabezado("MENU DE ADMINISTRADOR");
        cout << "  [1] Registrar docente\n";
        cout << "  [2] Crear curso (NRC)\n";
        cout << "  [3] Asignar docente a un NRC\n";
        cout << "  [4] Agregar horario a un NRC\n";
        cout << "  [5] Listar docentes\n";
        cout << "  [6] Reporte de asistencia\n";
        cout << "  [7] Marcar ausentes\n";
        cout << "  [8] Cargar datos de ejemplo\n";
        cout << "  [0] Volver\n\n";

        int opcion = Utilidades::leerOpcion("  Seleccione una opcion: ", 0, 8);
        switch (opcion) {
            case 1: registrarDocente(); break;
            case 2: crearCurso(); break;
            case 3: asignarDocente(); break;
            case 4: agregarHorario(); break;
            case 5: listarDocentes(); break;
            case 6: reporteAsistencia(); break;
            case 7: marcarAusentes(); break;
            case 8: cargarDatosEjemplo(); break;
            case 0: return;
        }
    }
}

void Menu::registrarDocente() {
    mostrarEncabezado("REGISTRAR DOCENTE");
    std::string cedula = Utilidades::leerCedula("  Cedula: ");
    std::string nombres = Utilidades::leerLetras("  Nombres: ", "Los nombres");
    std::string apellidos = Utilidades::leerLetras("  Apellidos: ", "Los apellidos");
    mostrarResultado(sistema.registrarDocente(cedula, nombres, apellidos));
    Utilidades::pausar();
}

void Menu::crearCurso() {
    mostrarEncabezado("CREAR CURSO");
    std::string nrc = Utilidades::leerNrc("  NRC: ");
    std::string materia = Utilidades::leerLetras("  Materia: ", "La materia");
    mostrarResultado(sistema.crearCurso(nrc, materia));
    Utilidades::pausar();
}

void Menu::asignarDocente() {
    mostrarEncabezado("ASIGNAR DOCENTE A UN NRC");
    std::string nrc = Utilidades::leerNrc("  NRC: ");
    const Curso* curso = sistema.obtenerCurso(nrc);
    if (curso == nullptr) {
        cout << "\n  [ERROR] No existe un curso con ese NRC.\n";
        Utilidades::pausar();
        return;
    }
    cout << "  Curso: " << curso->getMateria() << "\n";
    std::string cedula = Utilidades::leerCedula("  Cedula del docente: ");
    mostrarResultado(sistema.asignarDocente(nrc, cedula));
    Utilidades::pausar();
}

void Menu::agregarHorario() {
    mostrarEncabezado("AGREGAR HORARIO A UN NRC");
    std::string nrc = Utilidades::leerNrc("  NRC: ");
    const Curso* curso = sistema.obtenerCurso(nrc);
    if (curso == nullptr) {
        cout << "\n  [ERROR] No existe un curso con ese NRC.\n";
        Utilidades::pausar();
        return;
    }
    cout << "  Curso: " << curso->getMateria() << "\n\n";
    for (int d = 1; d <= 6; ++d) {
        cout << "  [" << d << "] " << nombreDia(static_cast<DiaSemana>(d)) << "\n";
    }
    int numeroDia = Utilidades::leerOpcion("  Dia: ", 1, 6);
    cout << "\n";
    for (int f = 1; f <= FranjaHorario::TOTAL; ++f) {
        cout << "  [" << f << "] " << FranjaHorario(f).aTexto() << "\n";
    }
    int numeroFranja = Utilidades::leerOpcion("  Franja: ", 1, FranjaHorario::TOTAL);
    mostrarResultado(sistema.agregarHorario(nrc, static_cast<DiaSemana>(numeroDia), numeroFranja));
    Utilidades::pausar();
}

void Menu::listarDocentes() const {
    mostrarEncabezado("LISTA DE DOCENTES");
    if (sistema.getDocentes().estaVacia()) {
        cout << "  No hay docentes registrados.\n";
    } else {
        cout << "  " << std::left << std::setw(12) << "Cedula" << std::setw(30) << "Docente" << "NRC asignados\n";
        sistema.getDocentes().recorrerAdelante([&](const Docente& d) {
            std::string nrcs;
            sistema.getCursos().recorrerAdelante([&](const Curso& c) {
                if (c.getCedulaDocente() == d.getCedula()) nrcs += c.getNrc() + " ";
            });
            cout << "  " << std::left << std::setw(12) << d.getCedula() << std::setw(30)
                 << d.nombreCompleto() << (nrcs.empty() ? "-" : nrcs) << "\n";
        });
        cout << "\n  Total: " << sistema.getDocentes().obtenerTamanio() << " docente(s).\n";
    }
    Utilidades::pausar();
}

void Menu::reporteAsistencia() const {
    mostrarEncabezado("REPORTE DE ASISTENCIA");
    if (sistema.getRegistros().estaVacia()) {
        cout << "  Aun no hay registros de asistencia.\n";
        Utilidades::pausar();
        return;
    }
    cout << "  [1] Del mas antiguo al mas reciente\n";
    cout << "  [2] Del mas reciente al mas antiguo\n\n";
    int orden = Utilidades::leerOpcion("  Recorrido: ", 1, 2);

    cout << "\n  " << std::left << std::setw(12) << "Fecha" << std::setw(8) << "NRC"
         << std::setw(22) << "Docente" << std::setw(13) << "Franja" << std::setw(8) << "Ingr."
         << std::setw(8) << "Sal." << "Estado\n";

    auto mostrarFila = [&](const RegistroAsistencia& r) {
        const Docente* d = sistema.obtenerDocente(r.getCedulaDocente());
        std::string nombre = (d != nullptr) ? d->nombreCompleto() : r.getCedulaDocente();
        if (nombre.size() > 20) nombre = nombre.substr(0, 20);
        std::string ingreso = r.tieneIngreso() ? FranjaHorario::formatoHora(r.getHoraIngreso()) : "--";
        std::string salida = r.tieneSalida() ? FranjaHorario::formatoHora(r.getHoraSalida()) : "--";
        cout << "  " << std::left << std::setw(12) << r.getFecha() << std::setw(8) << r.getNrc()
             << std::setw(22) << nombre << std::setw(13) << r.getFranja().aTexto()
             << std::setw(8) << ingreso << std::setw(8) << salida
             << estadoATexto(r.getEstado()) << "\n";
    };
    if (orden == 1) sistema.getRegistros().recorrerAdelante(mostrarFila);
    else sistema.getRegistros().recorrerAtras(mostrarFila);

    cout << "\n  Total de registros: " << sistema.getRegistros().obtenerTamanio() << "\n";
    Utilidades::pausar();
}

void Menu::marcarAusentes() {
    mostrarEncabezado("MARCAR AUSENTES");
    Instante ahora = instante();
    DiaSemana dia;
    if (!diaDesdeNumero(ahora.diaSemana, dia)) {
        cout << "\n  [ERROR] Hoy es domingo: no hay clases.\n";
        Utilidades::pausar();
        return;
    }
    int creados = sistema.marcarAusentes(dia, ahora.minutos, ahora.fecha);
    cout << "\n  Se marcaron " << creados << " clase(s) como AUSENTE.\n";
    if (creados > 0) guardarCambios();
    Utilidades::pausar();
}

void Menu::cargarDatosEjemplo() {
    mostrarEncabezado("DATOS DE EJEMPLO");
    sistema.registrarDocente("1710034065", "Carlos", "Mendoza");
    sistema.registrarDocente("0912345675", "Ana", "Paredes");
    sistema.crearCurso("14872", "Estructura de Datos");
    sistema.crearCurso("16210", "Redes de Datos");
    sistema.crearCurso("15033", "Programacion Avanzada");
    sistema.asignarDocente("14872", "1710034065");
    sistema.asignarDocente("16210", "1710034065");
    sistema.asignarDocente("15033", "0912345675");
    sistema.agregarHorario("14872", DiaSemana::LUNES, 1);
    sistema.agregarHorario("16210", DiaSemana::LUNES, 2);
    sistema.agregarHorario("15033", DiaSemana::LUNES, 1);
    cout << "  Se cargaron 2 docentes y 3 cursos:\n";
    cout << "    Carlos Mendoza (cedula 1710034065): NRC 14872 Lunes 07:00-09:00 y NRC 16210 Lunes 09:00-11:00\n";
    cout << "    Ana Paredes    (cedula 0912345675): NRC 15033 Lunes 07:00-09:00\n";
    cout << "  Tip: use la opcion 4 del menu principal para simular un Lunes a las 07:05.\n";
    guardarCambios();
    Utilidades::pausar();
}
