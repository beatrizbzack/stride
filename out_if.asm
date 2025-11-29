; Stride compiled assembly
; sensors available: TEMPO DISTANCIA BATAMENTOS
; memory slots start at @0
; --- begin program ---
; TERM number 0
MOV R0, 0
PUSH R0
; ASSIGN counter := (top of stack)
POP R0
STORE R0, @0
L1:
; TERM identifier counter -> slot @0
LOAD R0, @0
PUSH R0
; TERM number 3
MOV R0, 3
PUSH R0
; COND <
POP R1
POP R0
CMP R0, R1
JGE L1
; TERM string "loop:"
MOV R0, "loop:"
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
; TERM identifier counter -> slot @0
LOAD R0, @0
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
; TERM identifier counter -> slot @0
LOAD R0, @0
PUSH R0
; TERM number 1
MOV R0, 1
PUSH R0
; BINOP +
POP R1
POP R0
ADD R0, R1
PUSH R0
; ASSIGN counter := (top of stack)
POP R0
STORE R0, @0
JMP L1
L2:
; TERM string "done"
MOV R0, "done"
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
HALT
