#include "Sistema.h"
#include "Validador.h"

// ---------------------------------------------------------------- consultas

const Docente* Sistema::obtenerDocente(const std::string& cedula) const {
    return docentes.buscar([&](const Docente& d) { return d.getCedula() == cedula; });
}

const Curso* Sistema::obtenerCurso(const std::string& nrc) const {
    return cursos.buscar([&](const Curso& c) { return c.getNrc() == nrc; });
}

const ListaDoble<Docente>& Sistema::getDocentes() const { return docentes; }
const ListaDoble<Curso>& Sistema::getCursos() const { return cursos; }
const ListaDoble<RegistroAsistencia>& Sistema::getRegistros() const { return registros; }

Curso* Sistema::buscarCurso(const std::string& nrc) {
    return cursos.buscar([&](const Curso& c) { return c.getNrc() == nrc; });
}

RegistroAsistencia* Sistema::buscarRegistro(const std::string& fecha, const std::string& nrc,
                                            int numeroFranja) {
    return registros.buscar([&](const RegistroAsistencia& r) {
        return r.getFecha() == fecha && r.getNrc() == nrc &&
               r.getFranja().getNumero() == numeroFranja;
    });
}

// Curso (distinto de nrcExcluido) del mismo docente que ya tiene clase en ese dia y franja.
const Curso* Sistema::cursoEnConflicto(const std::string& cedula, DiaSemana dia,
                                       const FranjaHorario& franja,
                                       const std::string& nrcExcluido) const {
    return cursos.buscar([&](const Curso& c) {
        return c.getCedulaDocente() == cedula && c.getNrc() != nrcExcluido &&
               c.tieneCruce(dia, franja);
    });
}

std::string Sistema::mensajeCruce(const std::string& cedula, const Curso& otro, DiaSemana dia,
                                  const FranjaHorario& franja) const {
    const Docente* d = obtenerDocente(cedula);
    std::string nombre = (d != nullptr) ? d->nombreCompleto() : cedula;
    return "Cruce de horarios: " + nombre + " ya dicta el NRC " + otro.getNrc() + " el " +
           nombreDia(dia) + " de " + franja.aTexto() + ".";
}

// ---------------------------------------------------------------- administrador

Resultado Sistema::registrarDocente(const std::string& cedula, const std::string& nombres,
                                    const std::string& apellidos) {
    std::string motivo = Validador::motivoCedulaInvalida(cedula);
    if (!motivo.empty()) return Resultado::error(motivo);
    if (!Validador::soloLetras(nombres))
        return Resultado::error("Los nombres solo deben contener letras (sin numeros ni simbolos).");
    if (!Validador::soloLetras(apellidos))
        return Resultado::error("Los apellidos solo deben contener letras (sin numeros ni simbolos).");
    if (obtenerDocente(cedula) != nullptr)
        return Resultado::error("Ya existe un docente registrado con esa cedula.");

    docentes.insertarFinal(Docente(cedula, nombres, apellidos));
    return Resultado::ok("Docente registrado correctamente.");
}

Resultado Sistema::crearCurso(const std::string& nrc, const std::string& materia) {
    std::string motivo = Validador::motivoNrcInvalido(nrc);
    if (!motivo.empty()) return Resultado::error(motivo);
    if (!Validador::soloLetras(materia))
        return Resultado::error("La materia solo debe contener letras (sin numeros ni simbolos).");
    if (obtenerCurso(nrc) != nullptr)
        return Resultado::error("Ya existe un curso con ese NRC.");

    cursos.insertarFinal(Curso(nrc, materia));
    return Resultado::ok("Curso creado: NRC " + nrc + " - " + materia + ". Aun sin docente asignado.");
}

Resultado Sistema::asignarDocente(const std::string& nrc, const std::string& cedula) {
    Curso* curso = buscarCurso(nrc);
    if (curso == nullptr) return Resultado::error("No existe un curso con ese NRC.");
    const Docente* docente = obtenerDocente(cedula);
    if (docente == nullptr)
        return Resultado::error("No existe un docente con esa cedula. Registrelo primero.");

    // Los horarios que ya tiene el curso no deben chocar con otros cursos del docente.
    std::string conflicto;
    curso->getHorarios().recorrerAdelante([&](const HorarioCurso& h) {
        if (!conflicto.empty()) return;
        const Curso* otro = cursoEnConflicto(cedula, h.getDia(), h.getFranja(), nrc);
        if (otro != nullptr) conflicto = mensajeCruce(cedula, *otro, h.getDia(), h.getFranja());
    });
    if (!conflicto.empty()) return Resultado::error(conflicto);

    curso->asignarDocente(cedula);
    return Resultado::ok("Docente " + docente->nombreCompleto() + " asignado al NRC " + nrc + ".");
}

Resultado Sistema::agregarHorario(const std::string& nrc, DiaSemana dia, int numeroFranja) {
    Curso* curso = buscarCurso(nrc);
    if (curso == nullptr) return Resultado::error("No existe un curso con ese NRC.");
    if (!FranjaHorario::esNumeroValido(numeroFranja))
        return Resultado::error("La franja debe estar entre 1 y 7.");

    FranjaHorario franja(numeroFranja);
    if (curso->tieneCruce(dia, franja))
        return Resultado::error("Ese curso ya tiene asignado ese dia y esa franja.");

    if (curso->tieneDocente()) {
        const Curso* otro = cursoEnConflicto(curso->getCedulaDocente(), dia, franja, nrc);
        if (otro != nullptr)
            return Resultado::error(mensajeCruce(curso->getCedulaDocente(), *otro, dia, franja));
    }

    curso->agregarHorario(dia, franja);
    return Resultado::ok("Sin cruces. Horario agregado: " + nombreDia(dia) + " " + franja.aTexto());
}

int Sistema::marcarAusentes(DiaSemana dia, int minutos, const std::string& fecha) {
    int creados = 0;
    for (int f = 1; f <= FranjaHorario::TOTAL; ++f) {
        FranjaHorario franja(f);
        if (minutos < franja.inicioVentanaSalida()) continue;  // la clase aun no llega a su cierre

        cursos.recorrerAdelante([&](const Curso& curso) {
            if (!curso.tieneDocente() || !curso.tieneCruce(dia, franja)) return;
            if (buscarRegistro(fecha, curso.getNrc(), f) != nullptr) return;
            RegistroAsistencia registro(fecha, curso.getNrc(), curso.getCedulaDocente(), franja);
            registro.marcarAusente();
            registros.insertarFinal(registro);
            ++creados;
        });
    }
    return creados;
}

// ---------------------------------------------------------------- docente

// Valida la cedula, comprueba que el docente exista y localiza la clase que le toca ahora.
bool Sistema::ubicarClase(const std::string& cedula, DiaSemana dia, int minutos,
                          Curso*& curso, FranjaHorario& franja, std::string& error) {
    std::string motivo = Validador::motivoCedulaInvalida(cedula);
    if (!motivo.empty()) {
        error = motivo;
        return false;
    }
    if (obtenerDocente(cedula) == nullptr) {
        error = "No existe un docente registrado con esa cedula. Pida al administrador que lo registre.";
        return false;
    }
    int numero = FranjaHorario::numeroParaMinuto(minutos);
    if (numero == 0) {
        error = "Fuera del horario de clases (07:00 a 21:00).";
        return false;
    }
    franja = FranjaHorario(numero);
    curso = cursos.buscar([&](const Curso& c) {
        return c.getCedulaDocente() == cedula && c.tieneCruce(dia, franja);
    });
    if (curso == nullptr) {
        error = "No tiene clase asignada el " + nombreDia(dia) + " en la franja " +
                franja.aTexto() + ".";
        return false;
    }
    return true;
}

Resultado Sistema::registrarIngreso(const std::string& cedula, DiaSemana dia, int minutos,
                                    const std::string& fecha) {
    Curso* curso = nullptr;
    FranjaHorario franja;
    std::string error;
    if (!ubicarClase(cedula, dia, minutos, curso, franja, error)) return Resultado::error(error);

    RegistroAsistencia* existente = buscarRegistro(fecha, curso->getNrc(), franja.getNumero());
    if (existente != nullptr) {
        if (!existente->tieneIngreso())
            return Resultado::error("Ya fue marcado como AUSENTE en esta franja; no puede registrar ingreso.");
        return Resultado::error("Ya registro su ingreso en esta franja a las " +
                                FranjaHorario::formatoHora(existente->getHoraIngreso()) +
                                ". No es posible registrarse dos veces.");
    }
    if (minutos >= franja.inicioVentanaSalida())
        return Resultado::error("Ya es horario de salida; el ingreso ya no se puede registrar.");

    RegistroAsistencia registro(fecha, curso->getNrc(), cedula, franja);
    registro.registrarIngreso(minutos);
    registros.insertarFinal(registro);

    std::string mensaje = "Ingreso registrado a las " + FranjaHorario::formatoHora(minutos) +
                          " - NRC " + curso->getNrc() + " (" + curso->getMateria() + "), franja " +
                          franja.aTexto() + ". Estado: " + estadoATexto(registro.getEstado());
    if (registro.getEstado() == EstadoAsistencia::ATRASO)
        mensaje += " (fuera de la ventana de ingreso, hasta " +
                   FranjaHorario::formatoHora(franja.limiteIngresoPuntual()) + ")";
    return Resultado::ok(mensaje);
}

Resultado Sistema::registrarSalida(const std::string& cedula, DiaSemana dia, int minutos,
                                   const std::string& fecha) {
    Curso* curso = nullptr;
    FranjaHorario franja;
    std::string error;
    if (!ubicarClase(cedula, dia, minutos, curso, franja, error)) return Resultado::error(error);

    RegistroAsistencia* registro = buscarRegistro(fecha, curso->getNrc(), franja.getNumero());
    if (registro == nullptr || !registro->tieneIngreso())
        return Resultado::error("No tiene un ingreso registrado en esta franja; no puede registrar salida.");
    if (registro->tieneSalida())
        return Resultado::error("Ya registro su salida en esta franja a las " +
                                FranjaHorario::formatoHora(registro->getHoraSalida()) + ".");
    if (minutos < franja.inicioVentanaSalida())
        return Resultado::error("La salida solo se puede registrar entre las " +
                                FranjaHorario::formatoHora(franja.inicioVentanaSalida()) + " y las " +
                                FranjaHorario::formatoHora(franja.finEnMinutos() - 1) + ".");

    registro->registrarSalida(minutos);
    return Resultado::ok("Salida registrada a las " + FranjaHorario::formatoHora(minutos) +
                         " - NRC " + curso->getNrc() + ", franja " + franja.aTexto() + ".");
}

// ---------------------------------------------------------------- carga desde archivo

Resultado Sistema::cargarRegistro(const RegistroAsistencia& registro) {
    if (obtenerDocente(registro.getCedulaDocente()) == nullptr)
        return Resultado::error("el registro es de un docente que no existe");
    if (obtenerCurso(registro.getNrc()) == nullptr)
        return Resultado::error("el registro es de un curso que no existe");

    const FranjaHorario& franja = registro.getFranja();
    if (registro.tieneIngreso()) {
        int ingreso = registro.getHoraIngreso();
        if (!franja.contiene(ingreso) || ingreso >= franja.inicioVentanaSalida())
            return Resultado::error("la hora de ingreso no corresponde a la franja");
    }
    if (registro.tieneSalida()) {
        int salida = registro.getHoraSalida();
        if (!registro.tieneIngreso() || !franja.contiene(salida) ||
            salida < franja.inicioVentanaSalida() || salida < registro.getHoraIngreso())
            return Resultado::error("la hora de salida no es valida");
    }
    if (buscarRegistro(registro.getFecha(), registro.getNrc(), franja.getNumero()) != nullptr)
        return Resultado::error("ya existe un registro para esa fecha, NRC y franja");

    registros.insertarFinal(registro);
    return Resultado::ok("Registro cargado.");
}
