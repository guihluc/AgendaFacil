#include "agenda.hpp"

#include <stdexcept>

namespace {

const int TAMANHO_DATA = 10;
const int MINUTOS_POR_HORA = 60;

bool ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

int diasNoMes(int ano, int mes) {
    const int DIAS_POR_MES[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ehBissexto(ano)) {
        return 29;
    }
    return DIAS_POR_MES[mes - 1];
}

bool soTemDigitos(const std::string& texto) {
    for (char c : texto) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

void validarTexto(const std::string& valor, const std::string& campo) {
    if (valor.empty()) {
        throw std::invalid_argument("O campo " + campo + " nao pode ser vazio");
    }
}

void validarData(const std::string& data) {
    bool formatoCerto = data.size() == TAMANHO_DATA && data[4] == '-' &&
                        data[7] == '-' && soTemDigitos(data.substr(0, 4)) &&
                        soTemDigitos(data.substr(5, 2)) &&
                        soTemDigitos(data.substr(8, 2));
    if (!formatoCerto) {
        throw std::invalid_argument("Data fora do formato AAAA-MM-DD: \"" +
                                    data + "\"");
    }

    int ano = std::stoi(data.substr(0, 4));
    int mes = std::stoi(data.substr(5, 2));
    int dia = std::stoi(data.substr(8, 2));

    if (mes < 1 || mes > 12) {
        throw std::invalid_argument("Mes invalido na data \"" + data + "\"");
    }
    if (dia < 1 || dia > diasNoMes(ano, mes)) {
        throw std::invalid_argument("Dia invalido na data \"" + data + "\"");
    }
}

void validarDuracao(int duracao) {
    if (duracao < DURACAO_MINIMA || duracao > DURACAO_MAXIMA) {
        throw std::invalid_argument(
            "Duracao deve ficar entre " + std::to_string(DURACAO_MINIMA) +
            " e " + std::to_string(DURACAO_MAXIMA) +
            " minutos, recebido: " + std::to_string(duracao));
    }
    if (duracao % PASSO_DURACAO != 0) {
        throw std::invalid_argument(
            "Duracao deve ser multipla de " + std::to_string(PASSO_DURACAO) +
            ", recebido: " + std::to_string(duracao));
    }
}

void validarHorario(int hora, int duracao) {
    if (hora < HORA_ABERTURA || hora >= HORA_FECHAMENTO) {
        throw std::invalid_argument(
            "Hora de inicio deve ficar entre " + std::to_string(HORA_ABERTURA) +
            " e " + std::to_string(HORA_FECHAMENTO - 1) +
            ", recebido: " + std::to_string(hora));
    }
    validarDuracao(duracao);

    int minutosDoFim = hora * MINUTOS_POR_HORA + duracao;
    if (minutosDoFim > HORA_FECHAMENTO * MINUTOS_POR_HORA) {
        throw std::invalid_argument(
            "Atendimento das " + std::to_string(hora) + "h com " +
            std::to_string(duracao) + " minutos termina depois das " +
            std::to_string(HORA_FECHAMENTO) + "h");
    }
}

}  // namespace

bool Agenda::existeConflito(const std::string& profissional,
                            const std::string& data, int hora) const {
    for (const Atendimento& a : atendimentos_) {
        if (a.profissional == profissional && a.data == data && a.hora == hora) {
            return true;
        }
    }
    return false;
}

Atendimento* Agenda::buscar(const std::string& paciente,
                            const std::string& data, int hora) {
    for (Atendimento& a : atendimentos_) {
        if (a.paciente == paciente && a.data == data && a.hora == hora) {
            return &a;
        }
    }
    return nullptr;
}

bool Agenda::marcar(const std::string& paciente,
                    const std::string& profissional, const std::string& data,
                    int hora, int duracao) {
    validarTexto(paciente, "paciente");
    validarTexto(profissional, "profissional");
    validarData(data);
    validarHorario(hora, duracao);

    // Horario ocupado nao e entrada invalida, e uma situacao normal do dia a
    // dia da clinica. Por isso devolve false em vez de lancar excecao.
    if (existeConflito(profissional, data, hora)) {
        return false;
    }

    atendimentos_.push_back(
        {paciente, profissional, data, hora, duracao, STATUS_MARCADO});
    return true;
}

bool Agenda::remarcar(const std::string& paciente, const std::string& data,
                      int hora, const std::string& novaData, int novaHora) {
    validarTexto(paciente, "paciente");
    validarData(data);
    validarData(novaData);

    Atendimento* atendimento = buscar(paciente, data, hora);
    if (atendimento == nullptr) {
        throw std::runtime_error("Atendimento nao encontrado: paciente " +
                                 paciente + ", data " + data + ", hora " +
                                 std::to_string(hora));
    }

    validarHorario(novaHora, atendimento->duracao);

    if (existeConflito(atendimento->profissional, novaData, novaHora)) {
        return false;
    }

    atendimento->data = novaData;
    atendimento->hora = novaHora;
    return true;
}

std::vector<Atendimento> Agenda::listarPorData(const std::string& data) const {
    validarData(data);

    std::vector<Atendimento> resultado;
    for (const Atendimento& a : atendimentos_) {
        if (a.data == data) {
            resultado.push_back(a);
        }
    }
    return resultado;
}
