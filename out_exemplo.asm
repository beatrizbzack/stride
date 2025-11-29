; Stride compiled assembly
; sensors available: TEMPO DISTANCIA BATAMENTOS
; memory slots start at @0
; --- begin program ---
; TERM number 10
MOV R0, 10
PUSH R0
; ASSIGN x := (top of stack)
POP R0
STORE R0, @0
; TERM number 5
MOV R0, 5
PUSH R0
; ASSIGN y := (top of stack)
POP R0
STORE R0, @1
; TERM string "resultado:"
MOV R0, "resultado:"
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
; TERM identifier x -> slot @0
LOAD R0, @0
PUSH R0
; TERM identifier y -> slot @1
LOAD R0, @1
PUSH R0
; TERM number 2
MOV R0, 2
PUSH R0
; BINOP *
POP R1
POP R0
MUL R0, R1
PUSH R0
; BINOP +
POP R1
POP R0
ADD R0, R1
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
HALT
