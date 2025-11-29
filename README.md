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

## Entrega 1/12 - VM e testes

### VM Própria
Foi desenvolvida uma máquina virtual completa, em C, que interpreta o assembly gerado pelo parser. Fazendo com que o assembly gerado pela Stride seja totalmente executável pela VM personalizada. 

Recursos da VM:
- 3 registradores: R0, R1, R2
- Memória (@0, @1, …)
- Pilha de avaliação

Leitura de sensores:
- TEMPO
- DISTANCIA
- BATIMENTOS

Instruções suportadas:
- MOV, LOAD, STORE  
- PUSH, POP  
- ADD, SUB, MUL, DIV  
- CMP  
- JZ, JNZ, JGE, JGT, JLE, JLT, JMP  
- READ_SENSOR  
- PRINT  
- WAIT  
- HALT

### Testes Finais

1. x := 10
y := 5
mostre "resultado:"
mostre x + y * 2

Saída esperada:

resultado:
20

2. counter := 0
while counter < 3 do
  mostre "loop:"
  mostre counter
  counter := counter + 1
end
mostre "done"

resultado:

loop:
0
loop:
1
loop:
2
done

3. mostre "tempo atual:"
mostre tempo

mostre "distancia atual:"
mostre distancia

if distancia > 0 then
  mostre "distancia positiva"
else
  mostre "distancia zero"
end

corra
espere 1
pare

resultado:

tempo atual:
100
distancia atual:
5
distancia positiva
distancia zero
ACTION corra
[VM] waiting 1 seconds (simulated)
ACTION pare

### Como compilar:

1. make clean
make

- gera o Stride
- vm (máquina virtual)

2. executar:
- ./stride programa.stride out.asm
- ./vm out.asm

### Conclusão
A linguagem Stride implementa:

estruturas de controle (if, while)

expressões aritméticas completas

leitura de sensores simulados

ações do relógio

geração de assembly

execução em uma VM própria

Este projeto demonstra o ciclo completo de construção de um compilador + máquina virtual — alinhado aos objetivos da APS de Lógica da Computação, cobrindo análise léxica, análise sintática, geração de código e execução.
