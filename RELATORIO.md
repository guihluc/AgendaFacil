# Relatório — AgendaFácil (C++)

**Integrantes:** nome e matrícula
**Linguagem escolhida:** C++17

## 1. Diagnóstico do código-base

| # | Categoria | Anomalia (e onde) | Por que é um problema |
|---|-----------|-------------------|------------------------|
| 1 | Adequação e lógica | `add` só confere se a hora de início está entre 8 e 17. Um atendimento às 17h com 120 min termina às 19h e é aceito. | Viola a regra de expediente: o último atendimento deve terminar até as 18h. |
| 2 | Adequação e lógica | `add` não valida data nem duração máxima, nem se a duração é múltipla de 15 (só `dur > 0`). | Aceita dados que as regras de negócio proíbem. |
| 3 | Adequação e lógica | `rem` não valida o novo horário (`nh`) nem a nova data (`nd`). | É possível remarcar para as 23h ou para uma data sem sentido. |
| 4 | Adequação e lógica | O comentário de `lst` diz "lista os atendimentos do mês", mas a função filtra por um dia. | Comentário mentiroso leva a erro de uso. |
| 5 | Manutenibilidade | Nomes como `A`, `p`, `pr`, `d`, `h`, `dur`, `st`, `ag`, `add`, `rem`, `lst`. | Não dizem o que guardam. Para entender é preciso ler o código inteiro. |
| 6 | Manutenibilidade | A checagem de conflito está repetida em `add` e em `rem`. | Uma mudança na regra precisa ser feita em dois lugares e pode ser esquecida em um. |
| 7 | Manutenibilidade | Valores soltos: `8`, `18`, `"MARCADO"`. | Número mágico: ninguém sabe o que significa e a mudança exige caçar no código. |
| 8 | Manutenibilidade | `rem` busca, valida, altera e imprime ao mesmo tempo. `add` valida, agenda e imprime. | Mais de uma responsabilidade por função: difícil de testar e reutilizar. |
| 9 | Organização e estética | Quatro `if` aninhados em `add` (código em "seta"). | Difícil de ler. O caminho principal fica enterrado no meio. |
| 10 | Organização e estética | Cabeçalho com histórico de alterações, função comentada (`desmarcar`), TODO vencido e comentários como `// adiciona na lista`. | Ruído. O histórico é papel do Git e o código morto confunde. |
| 11 | Organização e estética | Tudo em um arquivo, com `using namespace std;` e mistura de português e inglês nos nomes. | Sem modularização, e `using namespace std` pode causar conflitos de nomes. |
| 12 | Programação defensiva e segurança | `catch (...) {}` vazio em `add`. | Engole qualquer erro sem avisar. |
| 13 | Programação defensiva e segurança | Entrada inválida (ex.: 20h) simplesmente não faz nada e nada é impresso. | Falha silenciosa: quem chamou nunca descobre que não marcou. |
| 14 | Programação defensiva e segurança | `rem` não avisa quando não encontra o atendimento. | Falha silenciosa de novo. |
| 15 | Programação defensiva e segurança | Nenhuma verificação de campos vazios (paciente, profissional). | Permite registros sem sentido. |
| 16 | Desempenho e recursos | Parâmetros `string` passados por valor em todas as funções. | Cópia desnecessária a cada chamada. |
| 17 | Desempenho e recursos | `vector<A> ag` global. | Estado compartilhado por todo o programa, qualquer função pode alterá-lo e não dá para ter duas agendas. |
| 18 | Desempenho e recursos | `int i < ag.size()` compara `int` com `size_t`, e `rem` continua percorrendo a lista depois de achar o atendimento. | Gera aviso com `-Wall` e trabalho desnecessário. |

## 2. Decisões de projeto

**Divisão em arquivos.** O código passou a ter `agenda.hpp`, `agenda.cpp` e `main.cpp`. A lógica fica na classe `Agenda`, e `main.cpp` só chama a agenda e imprime. Assim, quem agenda não imprime.

**Classe em vez de vetor global.** O vetor `ag` virou o atributo privado `atendimentos_` da classe `Agenda`. Só os métodos da classe mexem nele.

**Tipo `Atendimento`.** A struct `A` virou `Atendimento`, com campos de nome claro (`paciente`, `profissional`, `data`, `hora`, `duracao`, `status`).

**Conflito em um único lugar.** `existeConflito` é usado por `marcar` e `remarcar`. A busca do atendimento a remarcar ficou em `buscar`.

**Validações separadas.** `validarData`, `validarDuracao`, `validarHorario` e `validarTexto` ficam num `namespace` anônimo em `agenda.cpp`, porque são detalhes internos. Cada uma só valida. Nas funções públicas elas ficam no início, como cláusulas de guarda, o que também eliminou os `if` aninhados.

**Constantes.** 8, 18, 15, 120, 15 (passo) e `"MARCADO"` viraram constantes em MAIUSCULAS_COM_UNDERSCORE no `agenda.hpp`.

**Nomenclatura.** Funções e variáveis em camelCase, tipos em PascalCase, constantes em MAIUSCULAS_COM_UNDERSCORE. Os nomes do domínio ficam em português, sem acentos, para combinar com o texto do projeto.

**Horário ocupado devolve `false`, não lança exceção.** Conflito de horário é uma situação normal da clínica e não uma entrada inválida. Além disso, o programa de demonstração precisa continuar imprimindo "Horario ocupado".

**Exceções.** Entrada inválida lança `std::invalid_argument` com a mensagem dizendo o que está errado e o valor recebido. Atendimento inexistente em `remarcar` lança `std::runtime_error`. Não existe mais nenhum `catch` vazio. O `main` captura os dois tipos separadamente, mostra a mensagem em `std::cerr` e retorna 1.

**Passagem por referência.** Parâmetros `string` agora são `const std::string&`.

## 3. Mudanças de comportamento

O caminho feliz continua idêntico: a saída do programa de demonstração é a mesma de antes (conferida lado a lado). Mudou de propósito, só para entradas inválidas:

| Situação | Antes | Agora |
|----------|-------|-------|
| Marcar fora do expediente (ex.: 20h) | Nada acontecia | `std::invalid_argument` |
| Atendimento que termina depois das 18h | Era aceito | `std::invalid_argument` |
| Duração fora de 15–120 ou não múltipla de 15 | Aceita se `> 0` | `std::invalid_argument` |
| Data fora do formato AAAA-MM-DD (ou inexistente, como 2026-02-30) | Aceita | `std::invalid_argument` |
| Paciente ou profissional vazio | Aceito | `std::invalid_argument` |
| Remarcar para horário inválido | Aceito | `std::invalid_argument` |
| Remarcar atendimento que não existe | Nada acontecia | `std::runtime_error` |

## 4. Dívida técnica assumida

- O conflito considera só a hora de início exata, como no código original. Um atendimento de 120 min às 9h não bloqueia um às 10h do mesmo profissional.
- Remarcar para o mesmo dia e hora em que o atendimento já está retorna `false` ("Horario ocupado"), como antes.
- A busca é linear no vetor. Para uma clínica pequena é suficiente.
- Não existe função de desmarcar (o código comentado foi removido, e implementar seria funcionalidade nova).
- Os dados vivem só em memória, conforme as restrições do projeto.

## 5. Declaração de uso de IA

Foi usada uma ferramenta de IA (Claude) como apoio para gerar a primeira versão do código refatorado, do README e deste relatório.
