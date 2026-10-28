# AgendaFácil

Agenda de atendimentos de uma clínica pequena. Marca um atendimento, remarca e lista os atendimentos de um dia. Os dados ficam só em memória.

Projeto da Unidade 1 de DIM0501 (Boas Práticas de Programação), versão C++.

## Estrutura

```
src/agenda.hpp   declaração da classe Agenda, do tipo Atendimento e das constantes
src/agenda.cpp   regras de negócio e validações
src/main.cpp     programa de demonstração
Doxyfile         configuração da documentação
docs/            documentação gerada
RELATORIO.md     diagnóstico e decisões de projeto
```

## Requisitos

- g++ com suporte a C++17
- doxygen (só para gerar a documentação)

## Compilar e executar

```
g++ -std=c++17 -Wall src/*.cpp -o agenda
./agenda
```

Saída esperada:

```
Atendimento marcado para Ana
Horario ocupado
Atendimento marcado para Carla
Remarcado
14 Carla Dra. Souza MARCADO
```

## Gerar a documentação

Na pasta raiz do projeto:

```
doxygen Doxyfile
```

A documentação é criada em `docs/html`. Abra `docs/html/index.html` no navegador.

## Regras validadas

| Regra     | Definição |
|-----------|-----------|
| Expediente | Início entre 8h e 17h; o atendimento termina até as 18h |
| Duração   | De 15 a 120 minutos, múltiplo de 15 |
| Conflito  | Um profissional não tem dois atendimentos no mesmo dia e hora |
| Data      | Formato AAAA-MM-DD |

Entradas inválidas lançam `std::invalid_argument`. Remarcar um atendimento que não existe lança `std::runtime_error`. Horário ocupado não é erro: as funções devolvem `false`.

## Autores

- Nome do integrante: Luis Guilherme Lucas Barreto 
— Matrícula: 20250030776
