# stride
Linguagem de Programação de Alto Nível que simula um Smart Watch de corrida com funções básicas

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
               | "mostre", EXPRESSION ;

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