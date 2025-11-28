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
READ_SENSOR R0, DISTANCIA
PUSH R0
HALT
