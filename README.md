# Stride
Linguagem de Programação de Alto Nível que simula um Smart Watch de corrida com funções básicas

APS - LogComp
Por: Beatriz Borges Zackiewicz
---

# Stride – Linguagem de Programação
## Descrição
Stride é uma linguagem de programação de alto nível desenvolvida para simular as funcionalidades básicas de um Smart Watch focado em corrida. A linguagem permite aos usuários criar programas que podem monitorar atividades físicas, calcular distâncias percorridas, medir o tempo de corrida e fornecer feedback em tempo real.

## Funcionalidades / Ações básicas do relógio
- corra
- pare
- espere X tempo
- mostra Y mensagem

## Variáveis de somente leitura
- tempo
- distância
- bartmentos

## Variáveis de escrita
- declaradas pelo programador
- podem ser inputadas pelo usuário



##  EBNF
PROGRAM        = { DECLARATION | COMMAND } ;

DECLARATION    = IDENTIFIER ":=", EXPRESSION ;

COMMAND        = ASSIGNMENT | IF_STMT | WHILE_STMT | ACTION ;

ASSIGNMENT     = IDENTIFIER ":=", EXPRESSION ;

IF_STMT        = "if", EXPRESSION, "then", { COMMAND }, [ "else", { COMMAND } ], "end" ;

WHILE_STMT     = "while", EXPRESSION, [ "do" ], { COMMAND }, "end" ;

ACTION         = "corra"
               | "pare"
               | "espere", EXPRESSION
               | "mostre", EXPRESSION 
               | "mostre", STRING_LITERAL ;


EXPRESSION     = TERM, { OPERATOR, TERM } ;

TERM           = IDENTIFIER | NUMBER | STRING ;

OPERATOR       = "+" | "-" | "*" | "/" | "<" | ">" | "==" | "!=" | "<=" | ">=" ;

IDENTIFIER     = LETTER, { LETTER | DIGIT | "_" } ;

NUMBER         = DIGIT, { DIGIT } ;

STRING         = '"', { CHARACTER }, '"' ;

LETTER         = "a" | ... | "z" | "A" | ... | "Z" ;
DIGIT          = "0" | "1" | ... | "9" ;
CHARACTER      = ? qualquer símbolo exceto aspas ? ;

---


## Entrega 22/09 - Lexer e Parser (Flex e Bison)

### Arquivos principais
- `stride.l` — especificação Flex (lexer)
- `stride.y` — especificação Bison (parser)
- `build.ps1` — script PowerShell para gerar/compilar (usa `win_bison`, `win_flex`, `gcc`)

## Requisitos locais
- Windows com `win_flex` e `win_bison` (WinFlexBison) instalados e acessíveis no PATH.
- GCC (MSYS2/GCC) disponível como `gcc`.
- PowerShell para rodar `build.ps1`.

## Como compilar (PowerShell)
Abra PowerShell no diretório do projeto e execute:

- gera o parser (bison) e lexer (flex) e compila
.\build.ps1

Saída esperada:

1) bison -> stride.tab.c
2) flex -> lex.yy.c
3) gcc -> stride_parser.exe
Build concluído: .\stride_parser.exe

### Como testar (interativo)

Rode o parser sem argumento e digite comandos diretamente; finalize com Ctrl+Z (Windows) + Enter:

.\stride_parser.exe
corra;
mostre "teste";
# Ctrl+Z + Enter

Resultado esperado ao testar o Parser:

    PS C:\Users\bebec\OneDrive\Área de Trabalho\INSPER\7o Semestre (8o)\LogComp\stride> .\stride_parser.exe 
    mostre "teste";  
    action: mostre string => teste
    corra;           
    action: corra
