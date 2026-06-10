# Documentação Técnica — Como Compilar e Executar o Analisador

## 1) Visão Geral

Este projeto contém um **analisador léxico e sintático** para a linguagem MicroPascal, escrito em C. Os arquivos principais são:

| Arquivo         | Descrição                                                    |
|-----------------|--------------------------------------------------------------|
| `analisador.h`  | Definição das estruturas `Token`, `TokenVec` e `Simbolo`     |
| `analisador.c`  | Implementação do analisador léxico e ponto de entrada `main` |
| `sintatico.h`   | Definição da estrutura `Parser` e protótipo de `parse()`     |
| `sintatico.c`   | Implementação do analisador sintático recursivo descendente  |

O programa:
1. Lê um arquivo `.pas` de entrada (via argumento ou `codigo.pas` por padrão).
2. Realiza a análise léxica e gera os arquivos `result.lex`, `result.err` e `result.ts`.
3. Realiza a análise sintática sobre os tokens gerados.
4. Imprime `Analise sintatica concluida com sucesso!` em caso de sucesso, ou uma mensagem de erro no formato exigido em caso de falha.

---

## 2) Pré-requisitos

Para compilar o código, você precisará de um compilador C (GCC ou Clang) instalado no seu sistema.

### 🐧 Linux
O GCC normalmente já vem instalado nas principais distribuições. Para verificar ou instalar no Ubuntu/Debian:
```bash
gcc --version
sudo apt update && sudo apt install build-essential
```

### 🍎 macOS
O Clang é o compilador padrão fornecido pela Apple via Xcode Command Line Tools. Para verificar ou instalar via Terminal:
```bash
gcc --version
xcode-select --install
```

### 🪟 Windows
Recomenda-se instalar o GCC via MSYS2, MinGW-w64 ou TDM-GCC. Para verificar via PowerShell ou CMD:
```powershell
gcc --version
```
*(Caso não possua, instale um pacote MSYS2/MinGW e adicione o caminho do compilador nas variáveis de ambiente do Windows).*

---

## 3) Compilar

Na raiz do projeto, compile os dois arquivos `.c` juntos:

**Linux ou macOS:**
```bash
gcc analisador.c sintatico.c -o analisador
```

**Windows:**
```powershell
gcc analisador.c sintatico.c -o analisador.exe
```

*(Opcional: adicione `-g -Wall -Wextra` para ativar avisos e símbolos de depuração).*

---

## 4) Executar

O analisador aceita o arquivo de entrada como argumento de linha de comando.

**Linux ou macOS:**
```bash
./analisador micropascal/correto1.pas
```

**Windows:**
```powershell
.\analisador.exe micropascal\correto1.pas
```

Se nenhum argumento for fornecido, o programa tenta abrir `codigo.pas` na raiz do projeto:

```bash
./analisador
```

---

## 5) Arquivos de Teste

Os programas de teste estão na pasta `micropascal/`:

| Arquivo         | Tipo     | Descrição                                             |
|-----------------|----------|-------------------------------------------------------|
| `correto1.pas`  | ✅ Correto | Atribuição simples e expressões aritméticas          |
| `correto2.pas`  | ✅ Correto | Comando condicional `if-then-else`                   |
| `correto3.pas`  | ✅ Correto | Comando repetitivo `while` com bloco aninhado        |
| `erro1.pas`     | ❌ Errado  | Falta `;` após o nome do programa                    |
| `erro2.pas`     | ❌ Errado  | Uso de `:=` em vez de `:` na declaração de variáveis |
| `erro3.pas`     | ❌ Errado  | Fim de arquivo inesperado (falta o `end.`)           |

### Rodando todos os testes de uma vez (Linux/macOS):
```bash
for f in micropascal/*.pas; do
  echo "=== $f ==="
  ./analisador "$f" 2>&1
done
```

---

## 6) Saídas Geradas

Após cada execução bem-sucedida, três arquivos são gerados na raiz do projeto:

| Arquivo      | Conteúdo                                                            |
|--------------|---------------------------------------------------------------------|
| `result.lex` | Lista de tokens reconhecidos no formato `<NOME, valor> linha coluna`|
| `result.err` | Lista de erros léxicos encontrados                                  |
| `result.ts`  | Tabela de símbolos (identificadores declarados no programa)         |

### Saída de sucesso:
```
Analise sintatica concluida com sucesso!
Analise lexica finalizada.
Arquivos gerados: result.lex, result.err, result.ts
```

### Saída de erro sintático:
```
nn:token nao esperado [lex].
```
ou
```
nn:fim de arquivo não esperado.
```
onde `nn` é o número da linha onde o erro foi detectado e `lex` é o lexema encontrado.
