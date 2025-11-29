; Stride compiled assembly
; sensors available: TEMPO DISTANCIA BATAMENTOS
; memory slots start at @0
; --- begin program ---
; TERM string "tempo atual:"
MOV R0, "tempo atual:"
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
READ_SENSOR R0, TEMPO
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
; TERM string "distancia atual:"
MOV R0, "distancia atual:"
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
READ_SENSOR R0, DISTANCIA
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
READ_SENSOR R0, DISTANCIA
PUSH R0
; TERM number 0
MOV R0, 0
PUSH R0
; COND >
POP R1
POP R0
CMP R0, R1
JLE L0
; TERM string "distancia positiva"
MOV R0, "distancia positiva"
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
; TERM string "distancia zero"
MOV R0, "distancia zero"
PUSH R0
; ACTION mostre (pop and print)
POP R0
PRINT R0
; ACTION corra
PRINT "ACTION corra"
; TERM number 1
MOV R0, 1
PUSH R0
; ACTION espere (pop seconds)
POP R0
WAIT R0
; ACTION pare
PRINT "ACTION pare"
HALT
