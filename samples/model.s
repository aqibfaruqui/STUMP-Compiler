ORG 0
B main
SP      EQU     R6
stack   DATA    0x1200

ORG ___ (line number)
main:
    MOV R1, #0
    MOV R2, #0
    MOV R3, #0
    MOV R4, #0
    MOV R5, #0
    LD SP, [R0, #stack]

    ; x = __
    LD R1, [PC, #1]
    ADD PC, PC, #1
    DEFW ____ (x)

    ST R1, [SP]
    ADD R6, R6, #1

    ; y = __
    LD R1, [PC, #1]
    ADD PC, PC, #1
    DEFW ____ (y)

    ST R1, [SP]
    ADD SP, SP, #1


; =============================== Stack Space =============================== 

ORG 0x1000
DEFS 200

ORG 0x1200
