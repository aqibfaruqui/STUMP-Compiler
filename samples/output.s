ORG 0
B main

times:
    SUB R6, R6, #1
    LD R1, [R6]

    SUB R6, R6, #1
    LD R2, [R6]

    mult1:


    
main:
    LD R6, [PC, #1]
    ADD R6, R6, #1
    DEFW 0x1000
