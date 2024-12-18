ORG 0
B main


main:
    LD R6, [PC, #1]
    ADD PC, PC, #1
    DEFW ____ (stack address)

    LD R1, [PC, #1]
    ADD PC, PC, #1
    DEFW ____ (x)

    ST R1, [R6]
    ADD R6, R6, #1

    LD R1, [PC, #1]
    ADD PC, PC, #1
    DEFW ____ (y)

    ST R1, [R6]
    ADD R6, R6, #1


ORG 0x1000