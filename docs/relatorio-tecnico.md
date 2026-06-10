# Relatório Técnico — Analisador Léxico e Sintático MicroPascal

**Universidade Católica de Brasília**
**Disciplina:** Linguagens Formais, Autômatos e Compiladores
**Trabalho:** Parte 02 — Analisador Sintático

---

## 1. Estruturas de Dados

### 1.1 `Token` — `analisador.h`

Representa um único token produzido pelo analisador léxico.

| Campo    | Tipo            | Descrição                                                |
|----------|-----------------|----------------------------------------------------------|
| `nome`   | `char[32]`      | Classe do token (ex.: `KW_PROGRAM`, `ID`, `OP_ASS`)     |
| `tipo`   | `char[32]`      | Categoria textual (ex.: `palavra-chave`, `identificador`)|
| `valor`  | `char[64]`      | Lexema exato reconhecido no código-fonte                 |
| `linha`  | `int`           | Linha do código-fonte onde o token foi encontrado        |
| `coluna` | `int`           | Coluna do código-fonte onde o token foi encontrado       |

### 1.2 `TokenVec` — `analisador.h`

Vetor dinâmico que armazena a sequência completa de tokens produzida pelo analisador léxico. Implementado com realocação automática (`realloc`) quando a capacidade é excedida.

| Campo  | Tipo      | Descrição                                     |
|--------|-----------|-----------------------------------------------|
| `data` | `Token *` | Ponteiro para o array de tokens               |
| `size` | `int`     | Quantidade de tokens atualmente armazenados   |
| `cap`  | `int`     | Capacidade atual alocada                      |

### 1.3 `Simbolo` — `analisador.h`

Representa uma entrada na tabela de símbolos, que registra identificadores únicos encontrados no código-fonte.

| Campo    | Tipo        | Descrição                                  |
|----------|-------------|--------------------------------------------|
| `lexema` | `char[64]`  | Nome do identificador                      |
| `tipo`   | `char[32]`  | Tipo registrado (ex.: `identificador`)     |

### 1.4 `Parser` — `sintatico.h`

Estrutura de controle do analisador sintático. Encapsula o estado da análise.

| Campo | Tipo              | Descrição                                         |
|-------|-------------------|---------------------------------------------------|
| `v`   | `const TokenVec *`| Ponteiro para o vetor de tokens (somente leitura) |
| `i`   | `int`             | Índice do token corrente sendo analisado          |

---

## 2. Funções do Analisador Léxico (`analisador.c`)

| Função                   | Descrição                                                                                   |
|--------------------------|---------------------------------------------------------------------------------------------|
| `proximoToken(FILE *fp)` | Lê o arquivo caractere a caractere e retorna o próximo `Token` reconhecido pelo AFD          |
| `tokenize_all(FILE *fp)` | Chama `proximoToken` repetidamente até o `EOF`, acumulando todos os tokens em um `TokenVec` |
| `createSimpleToken(...)` | Cria e retorna um `Token` preenchido com os campos fornecidos                               |
| `createWordOrNumberToken(...)` | Classifica um lexema alfanumérico como palavra-chave, número real, inteiro ou identificador |
| `addTS(lexema, tipo)`    | Insere um identificador na tabela de símbolos, se ainda não existir                         |
| `tsExiste(lexema)`       | Verifica se um lexema já está na tabela de símbolos                                         |
| `initTS()`               | Pré-popula a tabela de símbolos com as palavras reservadas da linguagem                     |
| `writeLex(fpLex, token)` | Grava um token reconhecido no arquivo `.lex`                                                |
| `writeErr(fpErr, ...)`   | Grava um erro léxico no arquivo `.err`                                                      |
| `writeTS(fpTs)`          | Grava a tabela de símbolos completa no arquivo `.ts`                                        |
| `tv_free(TokenVec *v)`   | Libera a memória alocada pelo `TokenVec`                                                    |
| `toLower(dst, src, max)` | Converte uma string para minúsculas (usado na normalização de palavras-chave)               |

---

## 3. Funções do Analisador Sintático (`sintatico.c`)

O analisador sintático implementa um **Analisador Recursivo Descendente Preditivo**. Cada não-terminal da gramática corresponde a uma função estática em C.

### 3.1 Funções auxiliares de controle

| Função                            | Descrição                                                                                                           |
|-----------------------------------|---------------------------------------------------------------------------------------------------------------------|
| `cur(Parser *p)`                  | Retorna o token corrente sem avançar o índice                                                                       |
| `peek(Parser *p, char *nome)`     | Verifica se o token corrente é do tipo `nome`, **sem avançar**. Retorna 1 (verdadeiro) ou 0 (falso)                |
| `CasaToken(Parser *p, char *nome)`| **CasaToken:** Se o token corrente for do tipo `nome`, avança o índice e retorna 1. Caso contrário, retorna 0       |
| `expect(Parser *p, char *nome, char *msg)` | Chama `CasaToken`. Se falhar, dispara `perr` e encerra a análise                                      |
| `perr(Parser *p, char *msg)`      | Emite a mensagem de erro no formato exigido e encerra o processo com `exit(1)`                                      |

> A função `CasaToken` é a implementação do **procedimento `CasaToken`** descrito no enunciado do trabalho.

### 3.2 Funções de parse (não-terminais da gramática)

| Função                           | Não-terminal correspondente                    |
|----------------------------------|------------------------------------------------|
| `parse_program(Parser *p)`       | `<programa>`                                   |
| `parse_block(Parser *p)`         | `<bloco>`                                      |
| `parse_variable_declaration_block(Parser *p)` | `<parte de declarações de variáveis>` |
| `parse_variable_declaration(Parser *p)` | `<declaração de variáveis>`             |
| `parse_identifiers_list(Parser *p)` | `<lista de identificadores>`               |
| `parse_type(Parser *p)`          | `<tipo>`                                       |
| `parse_compound_command(Parser *p)` | `<comando composto>`                        |
| `parse_command(Parser *p)`       | `<comando>`                                    |
| `parse_atribution(Parser *p)`    | `<atribuição>`                                 |
| `parse_conditional_command(Parser *p)` | `<comando condicional>`                  |
| `parse_repetitive_command(Parser *p)` | `<comando repetitivo>`                    |
| `parse_expression(Parser *p)`    | `<expressão>`                                  |
| `parse_relation(Parser *p)`      | `<relação>`                                    |
| `parse_simple_expression(Parser *p)` | `<expressão simples>`                      |
| `parse_term(Parser *p)`          | `<termo>`                                      |
| `parse_factor(Parser *p)`        | `<fator>`                                      |
| `parse_variable(Parser *p)`      | `<variável>`                                   |

### 3.3 Ponto de entrada

| Função               | Descrição                                                                                   |
|----------------------|---------------------------------------------------------------------------------------------|
| `parse(TokenVec *v)` | Inicializa o `Parser` e invoca `parse_program`. Após a análise, verifica se não sobrou nenhum token além do `EOF` |

---

## 4. Testes Realizados

### 4.1 Programas Corretos

#### correto1.pas — Atribuição e expressões aritméticas

```pascal
program teste1;
var x, y: integer;
begin
    x := 10;
    y := x + 5 * 2;
end.
```

**Saída:**
```
Analise sintatica concluida com sucesso!
Analise lexica finalizada.
Arquivos gerados: result.lex, result.err, result.ts
```
✅ Passou sem erros.

---

#### correto2.pas — Comando condicional (if-then-else)

```pascal
program teste2;
var a, b: real;
    c: integer;
begin
    a := 5.5;
    b := 3.2;
    if a > b then
        c := 1
    else
        c := 0;
end.
```

**Saída:**
```
Analise sintatica concluida com sucesso!
Analise lexica finalizada.
Arquivos gerados: result.lex, result.err, result.ts
```
✅ Passou sem erros.

---

#### correto3.pas — Comando repetitivo (while) com bloco aninhado

```pascal
program teste3;
var i, sum: integer;
begin
    i := 0;
    sum := 0;
    while i < 10 do
    begin
        sum := sum + i;
        i := i + 1;
    end;
end.
```

**Saída:**
```
Analise sintatica concluida com sucesso!
Analise lexica finalizada.
Arquivos gerados: result.lex, result.err, result.ts
```
✅ Passou sem erros.

---

### 4.2 Programas com Erro Sintático

#### erro1.pas — Falta de `;` após o nome do programa

```pascal
program erro1
var x: integer;
begin
    x := 10;
end.
```

**Erro introduzido:** O `;` obrigatório após `program erro1` foi omitido.

**Saída:**
```
2:token nao esperado [var].
```
✅ Erro detectado corretamente na linha 2.

---

#### erro2.pas — Uso de `:=` em vez de `:` na declaração de variáveis

```pascal
program erro2;
var a, b := integer;
begin
    a := 5;
    b := 10;
end.
```

**Erro introduzido:** A gramática exige `:` para separar os identificadores do tipo. Foi usado `:=` por engano.

**Saída:**
```
2:token nao esperado [:=].
```
✅ Erro detectado corretamente na linha 2.

---

#### erro3.pas — Fim de arquivo inesperado (falta o `end.`)

```pascal
program erro3;
var x: integer;
begin
    x := 1;
```

**Erro introduzido:** O arquivo termina sem o `end.` que fecha o `begin`.

**Saída:**
```
5:fim de arquivo não esperado.
```
✅ Erro detectado corretamente na linha 5 com a mensagem alternativa exigida.

---

## 5. Conclusão

O analisador implementado cobre **integralmente** a gramática da linguagem MicroPascal definida no enunciado. A abordagem escolhida foi o **Analisador Recursivo Descendente Preditivo**, onde cada não-terminal possui uma função dedicada em C. A função `match` (equivalente ao `CasaToken` do enunciado) é responsável por comparar e avançar o token corrente. Em caso de erro, a mensagem é emitida no formato exato exigido (`nn:token nao esperado [lex].` ou `nn:fim de arquivo não esperado.`) e a execução é encerrada.
