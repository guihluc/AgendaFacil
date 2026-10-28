/**
 * @file main.cpp
 * @brief Programa de demonstracao do AgendaFacil.
 */
#include <iostream>
#include <stdexcept>

#include "agenda.hpp"

using std::cout;
using std::endl;
using std::string;

static void mostrarResultadoMarcacao(bool marcou, const string& paciente) {
    if (marcou) {
        cout << "Atendimento marcado para " << paciente << endl;
    } else {
        cout << "Horario ocupado" << endl;
    }
}

static void mostrarResultadoRemarcacao(bool remarcou) {
    if (remarcou) {
        cout << "Remarcado" << endl;
    } else {
        cout << "Horario ocupado" << endl;
    }
}

static void mostrarAtendimento(const Atendimento& a) {
    cout << a.hora << " " << a.paciente << " " << a.profissional << " "
         << a.status << endl;
}

/**
 * @brief Executa a demonstracao: marca tres atendimentos, remarca um e lista o dia.
 * @return 0 se tudo deu certo; 1 se alguma entrada foi invalida ou nao encontrada.
 */
int main() {
    try {
        Agenda agenda;

        mostrarResultadoMarcacao(
            agenda.marcar("Ana", "Dr. Silva", "2026-10-05", 9, 30), "Ana");
        mostrarResultadoMarcacao(
            agenda.marcar("Bruno", "Dr. Silva", "2026-10-05", 9, 30), "Bruno");
        mostrarResultadoMarcacao(
            agenda.marcar("Carla", "Dra. Souza", "2026-10-05", 14, 60), "Carla");

        mostrarResultadoRemarcacao(
            agenda.remarcar("Ana", "2026-10-05", 9, "2026-10-06", 10));

        for (const Atendimento& a : agenda.listarPorData("2026-10-05")) {
            mostrarAtendimento(a);
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Entrada invalida: " << e.what() << endl;
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro: " << e.what() << endl;
        return 1;
    }
    return 0;
}
