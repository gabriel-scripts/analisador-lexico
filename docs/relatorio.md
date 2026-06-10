# Documentação Técnica - Como rodar o analisador

## 1) Visão Geral
Este projeto contém um analisador léxico e sintático escrito em C. O ponto de entrada principal está no arquivo `analisador.c` e as regras de parse em `sintatico.c`.

No estado atual, o programa:
- Lê um arquivo chamado `codigo.pas` no diretório raiz do projeto.
- Faz a análise léxica e sintática completa.
- Imprime os tokens e a árvore de regras de produção no terminal.
- Gera os relatórios de erros ou as saídas esperadas.

## 2) Pré-requisitos

Para compilar o código, você precisará de um compilador C (como GCC ou Clang) instalado no seu sistema.

### 🐧 Linux
O GCC normalmente já vem instalado nas principais distribuições, ou pode ser facilmente obtido.
Para verificar ou instalar no Ubuntu/Debian:
```bash
gcc --version
sudo apt update && sudo apt install build-essential
```

### 🍎 macOS
O Clang é o compilador padrão fornecido pela Apple através do Xcode Command Line Tools.
Para verificar ou instalar via Terminal:
```bash
gcc --version
xcode-select --install
```

### 🪟 Windows
Recomenda-se instalar o GCC via MSYS2, MinGW-w64 ou TDM-GCC.
Para verificar via PowerShell ou CMD:
```powershell
gcc --version
```
*(Caso não possua, instale um pacote MSYS2/MinGW e adicione o caminho do compilador nas variáveis de ambiente do Windows).*

## 3) Estrutura Esperada
O programa abre o arquivo de entrada com nome fixo `codigo.pas`:

```c
fp = fopen("codigo.pas", "r");
```

Então o arquivo a ser testado deve existir na raiz do projeto, ao lado dos arquivos de código fonte. No repositório existem exemplos de testes na pasta `micropascal/`.

## 4) Preparar Arquivo de Entrada

A partir da raiz do projeto, você precisa copiar um dos testes para o arquivo `codigo.pas`.

**No Linux ou macOS (Terminal):**
```bash
cp micropascal/correto1.pas codigo.pas
```

**No Windows (PowerShell):**
```powershell
Copy-Item .\micropascal\correto1.pas .\codigo.pas
```

## 5) Compilar

Na raiz do projeto, utilize o comando apropriado para compilar os arquivos C juntos:

**No Linux ou macOS:**
```bash
gcc analisador.c sintatico.c -o analisador
```

**No Windows:**
```powershell
gcc analisador.c sintatico.c -o analisador.exe
```

*(Opcional: Se preferir gerar com símbolos de debug e avisos ativados, adicione `-g -Wall -Wextra` após o `gcc`).*

## 6) Executar

Execute o binário gerado na raiz do projeto.

**No Linux ou macOS:**
```bash
./analisador
```

**No Windows (PowerShell/CMD):**
```powershell
.\analisador.exe
```

### Saída Esperada:
- Impressão dos caracteres lidos.
- Impressão dos tokens identificados no console.
- Impressão sequencial de todas as regras de produção do analisador sintático (`<regra> ::= ...`).
- Conclusão com sucesso ou a impressão exata do erro, caso o arquivo teste seja inválido.
