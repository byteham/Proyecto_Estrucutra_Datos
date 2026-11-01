#pragma once
#include <string>

class Docente {
public:
    Docente(const std::string& cedula, const std::string& nombres, const std::string& apellidos);

    const std::string& getCedula() const;
    const std::string& getNombres() const;
    const std::string& getApellidos() const;
    std::string nombreCompleto() const;

private:
    std::string cedula;
    std::string nombres;
    std::string apellidos;
};
