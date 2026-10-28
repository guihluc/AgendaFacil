/**
 * @file agenda.hpp
 * @brief Agenda de atendimentos de uma clinica pequena.
 */
#ifndef AGENDA_HPP
#define AGENDA_HPP

#include <string>
#include <vector>

/// Primeira hora em que um atendimento pode comecar.
const int HORA_ABERTURA = 8;
/// Hora em que o expediente termina (nenhum atendimento passa daqui).
const int HORA_FECHAMENTO = 18;
/// Menor duracao permitida, em minutos.
const int DURACAO_MINIMA = 15;
/// Maior duracao permitida, em minutos.
const int DURACAO_MAXIMA = 120;
/// A duracao deve ser multipla deste valor, em minutos.
const int PASSO_DURACAO = 15;
/// Status de um atendimento recem marcado.
const std::string STATUS_MARCADO = "MARCADO";

/**
 * @brief Um atendimento agendado.
 */
struct Atendimento {
    std::string paciente;      ///< Nome do paciente.
    std::string profissional;  ///< Nome do profissional que atende.
    std::string data;  ///< Formato AAAA-MM-DD.
    int hora;          ///< Hora de inicio (8 a 17).
    int duracao;       ///< Duracao em minutos.
    std::string status;  ///< Situacao do atendimento (ex.: MARCADO).
};

/**
 * @brief Guarda os atendimentos em memoria e aplica as regras da clinica.
 */
class Agenda {
public:
    /**
     * @brief Marca um novo atendimento.
     * @param paciente Nome do paciente (nao pode ser vazio).
     * @param profissional Nome do profissional (nao pode ser vazio).
     * @param data Data no formato AAAA-MM-DD.
     * @param hora Hora de inicio, de 8 a 17.
     * @param duracao Duracao em minutos: de 15 a 120, multiplo de 15,
     *        terminando ate as 18h.
     * @return true se o atendimento foi marcado; false se o profissional
     *         ja tem atendimento nesse dia e hora.
     * @throw std::invalid_argument se algum parametro for invalido.
     */
    bool marcar(const std::string& paciente, const std::string& profissional,
                const std::string& data, int hora, int duracao);

    /**
     * @brief Muda a data e a hora de um atendimento ja marcado.
     * @param paciente Nome do paciente do atendimento.
     * @param data Data atual do atendimento (AAAA-MM-DD).
     * @param hora Hora atual do atendimento.
     * @param novaData Nova data (AAAA-MM-DD).
     * @param novaHora Nova hora de inicio, de 8 a 17.
     * @return true se foi remarcado; false se o profissional ja tem
     *         atendimento na nova data e hora.
     * @throw std::invalid_argument se algum parametro for invalido.
     * @throw std::runtime_error se nao existir o atendimento informado.
     */
    bool remarcar(const std::string& paciente, const std::string& data,
                  int hora, const std::string& novaData, int novaHora);

    /**
     * @brief Lista os atendimentos de um dia.
     * @param data Data no formato AAAA-MM-DD.
     * @return Os atendimentos do dia, na ordem em que foram marcados.
     * @throw std::invalid_argument se a data for invalida.
     */
    std::vector<Atendimento> listarPorData(const std::string& data) const;

private:
    std::vector<Atendimento> atendimentos_;

    bool existeConflito(const std::string& profissional,
                        const std::string& data, int hora) const;
    Atendimento* buscar(const std::string& paciente, const std::string& data,
                        int hora);
};

#endif
