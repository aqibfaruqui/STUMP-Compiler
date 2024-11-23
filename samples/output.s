; R0 = 0
; R1-R4
; R5 = return register
; R6 = stack pointer -> link at top
; R7 = PC

ORG 0;
BAL main;

/* Memory layout
 *   ORG 1;   from address 1
 *   head DATA 35; global var
 *   tail DATA 43; global var
 *   cherry_pos DATA 0;
 *   score DATA 0
 *   direction DATA 1;
 *
 *   ORG + no. globals;
 *   branch_main BAL main;
 *   branch_func1 BAL func1;
 *   branch_func2 BAL func2;
 *   branch_func3 BAL func3;
 *      
 *   ORG + no. globals + no. functions;
 *     
 *   func1:
 *      ...
 *   func2:
 *      ...
 *   func3:
 *      ...
 *  
 *   main:
 *      ...   
 *      BGE branch_func1;
 *
 *
 *      MOV lr, r7
 *      ADD lr, lr, #4
 *      BLE after
 *      ld r7 [r7]
 *      defw $100
 *      
 *      after
 *      ORG 0x100
 *          function...
 *
 *   call stack:
 *     func1 header
 *
 *
 *
 */


start