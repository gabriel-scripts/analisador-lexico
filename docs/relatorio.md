# Documentacao tecnica - como rodar o analisador lexico

## 1) Visao geral
Este projeto contem um analisador lexico escrito em C no arquivo `analisador.c`.

No estado atual, o programa:
- Le um arquivo chamado `codigo.pas` no diretorio raiz do projeto.
- Faz analise lexico-basica e imprime tokens no terminal.
- Ainda possui partes em desenvolvimento (ex.: escrita de arquivos de saida).

## 2) Pre-requisitos

### Windows
- Compilador C (GCC via MinGW-w64, MSYS2 ou TDM-GCC).
- PowerShell (ja disponivel no Windows).

Para validar se o GCC esta instalado:

    gcc --version

Se o comando nao existir, instale um pacote de GCC (ex.: MSYS2/MinGW) e reinicie o terminal.

### Linux/macOS
- GCC ou Clang instalado.

## 3) Estrutura esperada
O programa abre o arquivo de entrada com nome fixo `codigo.pas`:

    fp = fopen("codigo.pas", "r");

Entao o arquivo deve existir na raiz do projeto, ao lado de `analisador.c`.

No repositiorio existe um exemplo em:
- `micropascal/micropascal.pas`

## 4) Preparar arquivo de entrada (Windows PowerShell)
A partir da raiz do projeto:

    Copy-Item .\micropascal\micropascal.pas .\codigo.pas

## 5) Compilar
Na raiz do projeto:

    gcc analisador.c -o analisador.exe

Se preferir gerar com simbolos de debug:

    gcc -g -Wall -Wextra analisador.c -o analisador.exe

## 6) Executar
Na raiz do projeto:

    .\analisador.exe

Saida esperada:
- Impressao de caracteres lidos.
- Impressao de tokens identificados no console.
