#pragma once
#include <string>
#include "Curso.h"
#include "DiaSemana.h"
#include "Docente.h"
#include "FranjaHorario.h"
#include "ListaDoble.h"
#include "RegistroAsistencia.h"

// Resultado de una operacion: si salio bien y el mensaje para mostrar al usuario.
struct Resultado {
    bool exito;
    std::string mensaje;
    Resultado(bool e, const std::string& m) : exito(e), mensaje(m) {}
    static Resultado ok(const std::string& m) { return Resultado(true, m); }
    static Resultado error(const std::string& m) { return Resultado(false, m); }
};

// Logica del sistema. No imprime ni lee del teclado: eso lo hace Menu.
// El dia y la hora se reciben como parametros para poder probarlo en cualquier momento.
class Sistema {
public:
    // --- Administrador ---
    Resultado registrarDocente(const std::string& cedula, const std::string& nombres,
                               const std::string& apellidos);
    Resultado crearCurso(const std::string& nrc, const std::string& materia);
    Resultado asignarDocente(const std::string& nrc, const std::string& cedula);
    Resultado agregarHorario(const std::string& nrc, DiaSemana dia, int numeroFranja);
    // Crea registros AUSENTE para las clases cuya hora de salida ya empezo y no tienen ingreso.
    int marcarAusentes(DiaSemana dia, int minutos, const std::string& fecha);

    // --- Docente ---
    Resultado registrarIngreso(const std::string& cedula, DiaSemana dia, int minutos,
                               const std::string& fecha);
    Resultado registrarSalida(const std::string& cedula, DiaSemana dia, int minutos,
                              const std::string& fecha);

    // Agrega un registro ya armado (se usa al cargar el archivo). Comprueba que sea coherente.
    Resultado cargarRegistro(const RegistroAsistencia& registro);

    // --- Consultas ---
    const Docente* obtenerDocente(const std::string& cedula) const;
    const Curso* obtenerCurso(const std::string& nrc) const;
    const ListaDoble<Docente>& getDocentes() const;
    const ListaDoble<Curso>& getCursos() const;
    const ListaDoble<RegistroAsistencia>& getRegistros() const;

private:
    ListaDoble<Docente> docentes;
    ListaDoble<Curso> cursos;
    ListaDoble<RegistroAsistencia> registros;

    Curso* buscarCurso(const std::string& nrc);
    RegistroAsistencia* buscarRegistro(const std::string& fecha, const std::string& nrc,
                                       int numeroFranja);
    const Curso* cursoEnConflicto(const std::string& cedula, DiaSemana dia,
                                  const FranjaHorario& franja,
                                  const std::string& nrcExcluido) const;
    std::string mensajeCruce(const std::string& cedula, const Curso& otro, DiaSemana dia,
                             const FranjaHorario& franja) const;
    bool ubicarClase(const std::string& cedula, DiaSemana dia, int minutos,
                     Curso*& curso, FranjaHorario& franja, std::string& error);
};
