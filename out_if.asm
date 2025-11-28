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
POP R0
MOV R1, 0
CMP R0, R1
JZ L2
HALT
