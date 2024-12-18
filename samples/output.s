ORG 0
B main

main:
    LD R6, [PC, #1]
    ADD R6, R6, #1
    DEFW 0x1000
