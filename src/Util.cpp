#include "../include/Util.hpp"
#include <algorithm> // Para std::find_if
#include <cctype>   // Para std::isspace
#include <locale>

// Função trim definida apenas aqui
std::string trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r");
    auto end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}
