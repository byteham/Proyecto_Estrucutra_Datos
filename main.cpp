#include <exception>
#include <iostream>
#include "Menu.h"

int main() {
    try {
        Menu menu;
        menu.ejecutar();
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Error inesperado.\n";
        return 1;
    }
    return 0;
}
