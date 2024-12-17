; ================================== Initialisation ===================================
; |
; | Register layout
; | R0 = 0
; | R1 = Return Register
; | R2 =
; | R3 =
; | R4 = Peripheral input address e.g. strings, colours
; | R5 = Peripheral address e.g. LCD display, LED matrix
; | R6 = Stack Pointer
; | R7 = Program Counter
; |
; =============================== Variable Declarations =============================== 

initialisation:
    ORG 0
    B main

    ; Stack Pointer
    stack DATA 0x800

    ; Branch table
    branch_table DATA 0x900

    ; String/Character tables
    string_table DATA 0x1000

    ; Peripheral tables
    led_table DATA 0x1200
    lcd_table DATA 0x1210
    keypad_table DATA 0x1220



; ================================ Game Initialisation ================================

main:
    LD R1, [R0, #lcd_table]
    ADD R1, R1, #4
    LD R2, [R1]
    SUB R1, R1, #1
    ST R2, [R1]                 ; Reset current LCD pointer in memory

    LD R1, [R0, #led_table]
    LD R2, [R1]
    ADD R1, R1, #2
    ST R2, [R1]                 ; Reset current LED pointer in memory

    MOV R1, #0
    MOV R2, #0
    MOV R3, #0
    MOV R4, #0
    MOV R5, #0
    LD R6, [R0, #stack]

    ;-----> Clearing LED Matrix
    LD R4, [R0, #led_table]     
    ADD R5, R4, #1
    LD R4, [R4]                 ; R4 -> LED Matrix pointer begin
    LD R5, [R5]                 ; R5 -> LED Matrix pointer end             
    clear_led:
        CMP R4, R5
        BGT exit_clearled
        ST R0, [R4]             
        ADD R4, R4, #1          
        B clear_led
    exit_clearled

    ;-----> Clearing LCD Display
    LD R4, [R0, #lcd_table]
    ADD R5, R4, #1
    LD R4, [R4]                 ; R4 -> LCD Display pointer begin
    LD R5, [R5]                 ; R5 -> LCD Display pointer end
    clear_lcd:
        CMP R4, R5
        BGT exit_clcd
        ST R0, [R4]    
        ADD R4, R4, #1  
        B clear_lcd     
    exit_clcd


    ;-----> Writing start message
    LD R4, [R0, #string_table]   
    LD R5, [R0, #lcd_table]
    LD R4, [R4]                 ; R4 -> start_menu string
    LD R5, [R5]                 ; R5 -> LCD display pointer
    print_start:
        LD R1, [R4]     ; R1 -> string byte
        CMP R1, R0      ; check for '\0' exit character
        BEQ exit_ps     ;       + exit if end of string
        ST R1, [R5]     ; string byte in LCD address 
        ADD R4, R4, #1  ; increment string pointer
        ADD R5, R5, #1  ; increment LCD display pointer
        B print_start   
    exit_ps

    ;-----> Press * to start game
    start_loop:
        LD R4, [R0, #keypad_table]
        ADD R5, R4, #1
        LD R4, [R4]             
        LD R4, [R4]             ; R4 -> Keypad input
        LD R5, [R5]             ; R5 -> asterisk
        CMP R4, R5              
        BNE start_loop          
    B start


; ==================================== Game Start =====================================

start:
    ;-----> Delay to clear keypad input
    LD R1, [R0, #led_table]
    LD R1, [R1]
    delay:
        SUB R1, R1, #1
        CMP R0, R1
        BNE delay

    ;-----> Clearing LCD Display
    LD R4, [R0, #lcd_table]
    ADD R5, R4, #1
    LD R4, [R4]                 ; R4 -> LCD Display pointer begin
    LD R5, [R5]                 ; R5 -> LCD Display pointer end
    clear_lcd2:
        CMP R4, R5
        BGT exit_clcd2
        ST R0, [R4]    
        ADD R4, R4, #1  
        B clear_lcd2
    exit_clcd2

    ;-----> Displaying 'Score: 00'
    LD R4, [R0, #string_table] ; R4 -> string pointer
    LD R4, [R4, #1]            ; R4 -> score string
    LD R5, [R0, #lcd_table]    ; R5 -> LCD display pointer
    LD R5, [R5, #2]
    print_score:
        LD R1, [R4]     ; R1 -> string byte
        CMP R1, R0      ; check for '\0' exit character
        BEQ exit_ps2    ;       + exit if end of string
        ST R1, [R5]     ; string byte in LCD address
        ADD R4, R4, #1  ; increment string pointer
        ADD R5, R5, #1  ; increment LCD display pointer
        B print_score
    exit_ps2


; ===================================== Game Loop =====================================

game_loop:
    ORG 0x0055
    question_generation:
        ;-----> Clearing input on LCD Display
        LD R4, [R0, #lcd_table]
        ADD R4, R4, #4
        ADD R5, R4, #1
        LD R4, [R4]                 ; R4 -> LCD Display pointer begin
        LD R5, [R5]                 ; R5 -> LCD Display input end
        clear_lcd3:
            CMP R4, R5
            BGT exit_clcd3
            ST R0, [R4]    
            ADD R4, R4, #1  
            B clear_lcd3
        exit_clcd3

        ;-----> Resetting input pointer
        LD R4, [R0, #lcd_table]
        ADD R4, R4, #3
        ADD R5, R4, #1
        LD R5, [R5]
        ST R5, [R4]

        B generate_num_calls
        ;-----> Function: Generating random numbers
        ;          param: _
        ;         return: R5 = random number 
        counter DEFW 0xFFA4
        bitmask DEFW 127
        limit DEFW 100
        generate_num:
            LD R4, bitmask   
            LD R5, counter          ; Free running counter
            LD R5, [R5]             ; R5 = Random number
            AND R5, R5, R4          ; Random number <= 127
            LD R4, limit
            CMP R5, R4              ; Regenerate if random >= 100
            BGT generate_num
            CMP R5, R0
            BEQ generate_num        ; Regenerate if random == 0
            SUB R6, R6, #1          ; Pop return address to PC
            LD PC, [R6]

        ;-----> Function calls: generate_num()
        generate_num_calls:
            ADD R1, PC, #3
            ST R1, [R6]         
            ADD R6, R6, #1          
            B generate_num          
            MOV R2, R5              ; R2 = First random number

            ADD R1, PC, #3
            ST R1, [R6]             
            ADD R6, R6, #1          
            B generate_num          
            MOV R3, R5              ; R3 = Second random number

            ADD R1, R2, R3
            ST R1, [R6]
            ADD R6, R6, #1          ; Stack result

        B generate_question
        ;-----> Function: Integer division and modulo by 10
        ;          param: number
        ;         return: R1 = modulo, R2 = division
        ascii_output DEFW 48
        stack_digits:
            SUB R6, R6, #1
            LD R1, [R6]             ; R1 (param) -> Modulo
            MOV R2, R0              ; R2 -> Division
            div:
                CMP R1, #10
                BLT div_exit
                SUB R1, R1, #10
                ADD R2, R2, #1
                B div
            div_exit:
                LD R4, ascii_output
                ADD R1, R1, R4
                ADD R2, R2, R4
                SUB R6, R6, #1
                LD R5, [R6]         ; Pop return address
                ST R1, [R6]         ; Push modulo
                ADD R6, R6, #1
                ST R2, [R6]         ; Push division
                ADD R6, R6, #1
                MOV PC, R5          ; Write return address to PC


        ;-----> Function: Generate and write question to LCD
        ;          param: Two random numbers
        ;         return: question answer + (LCD write)
        space DEFW 0x20
        addition DEFW 0x2B
        generate_question:

            ;-----> Function call: Random number 1 -> ASCII in stack
            ADD R1, PC, #5
            ST R1, [R6]     
            ADD R6, R6, #1
            ST R2, [R6]          
            ADD R6, R6, #1 
            B stack_digits          

            ;-----> Push ASCIIs to stack: ' ', '+', ' '
            LD R1, space
            ST R1, [R6]
            ADD R6, R6, #1
            LD R1, addition
            ST R1, [R6]
            ADD R6, R6, #1
            LD R1, space
            ST R1, [R6]
            ADD R6, R6, #1

            ;-----> Function call: Random number 2 -> ASCII in stack
            ADD R1, PC, #5
            ST R1, [R6]        
            ADD R6, R6, #1
            ST R3, [R6]
            ADD R6, R6, #1 
            B stack_digits        
  
            ;-----> Pop string and write to LCD
            LD R5, [R0, #lcd_table]
            LD R5, [R5]                 ; R5 -> LCD display pointer
            MOV R1, #7
            write_question:
                CMP R1, R0
                BEQ exit_wq
                SUB R6, R6, #1          ; Pop from stack
                LD R4, [R6]             ; R4 -> String byte
                ST R4, [R5]             ; Write to LCD
                ADD R5, R5, #1          ; Increment LCD pointer
                SUB R1, R1, #1          ; Decrement for loop
                B write_question
            exit_wq
        B question_answer


    ORG 0x00C0
    question_answer:
        B input_call
        ;-----> Function: Reading keypad input
        ;          param: _
        ;         return: R1 = 0-11 (0-9/*/#) 
        keypad_input DEFW 0xFF94
        input:
            input_loop:
                LD R5, keypad_input
                LD R5, [R5]         ; R5 -> keypad input
                CMP R5, R0
                BEQ input_loop
            MOV R3, R0
            keypad_value:
                ORS R5, R5, R0, ROR ; Count rotations
                BMI exit_kv         ; ( ͡° ͜ʖ ͡°) ROR rotates to negative
                ADD R3, R3, #1      ; R3 = input value
                B keypad_value
            exit_kv:
                LD R5, [R0, #led_table]
                input_delay:
                    SUB R1, R1, #1
                    CMP R0, R1
                    BNE input_delay
                LD R5, keypad_input
                LD R5, [R5]         ; R5 -> keypad input
                CMP R5, R0
                BNE input_delay
                SUB R6, R6, #1      ; Pop return address to PC
                LD PC, [R6]


        input_call:
            ADD R1, PC, #3
            ST R1, [R6]
            ADD R6, R6, #1
            B input

            ; R2 = result
            ; R3 = number

        LD R4, [R0, #branch_table]
        LD PC, [R4]

        ORG 0x00E0
        B rewrite_lcd
        ;-----> Function: Writing/Backspacing from LCD input
        ;          param: (0-9) or backspace
        ;         return: _  
        ascii_input DEFW 48
        rewrite_lcd:
            SUB R6, R6, #1
            LD R1, [R6]                 ; R1 = parameter
            LD R4, [R0, #lcd_table]
            ADD R4, R4, #3              ; R4 = Pointer to LCD Pointer
            CMP R1, #10
            BEQ backspace
            write_number:
                LD R5, ascii_input
                ADD R1, R1, R5          ; Number to ASCII
                LD R5, [R4]             ; R5 = LCD pointer
                LD R3, [R0, #lcd_table]
                    ADD R3, R3, #5          
                    LD R3, [R3]             ; R3 = input_max
                    CMP R5, R3              ; Check if 3 digits already written
                    BGE exit_rlcd           ; Yes? Don't write!
                ST R1, [R5]             
                ADD R5, R5, #1
                ST R5, [R4]             ; Update LCD pointer
                B exit_rlcd
            backspace:
                LD R5, [R4]             ; R5 = LCD pointer
                LD R3, [R0, #lcd_table]
                    ADD R3, R3, #4          ; R3 = input_min
                    LD R3, [R3]
                    CMP R5, R3              ; Check if 0 digits written
                    BLE exit_rlcd           ; Yes? Don't write!
                SUB R5, R5, #1
                ST R0, [R5]             
                ST R5, [R4]             ; Update LCD pointer
            exit_rlcd:
                SUB R6, R6, #1
                LD PC, [R6]

        ORG 0x0115
        B increment_score
        ;-----> Function: Updates score and LED Representation
        ;          param: Boolean to update score, LED colour value 
        ;         return: _
        curr_led DEFW 0x1202
        score_rewrite DATA 0xFF6F
        ascii_score DEFW 48
        increment_score:
            update_led:
                SUB R6, R6, #1
                LD R1, [R6]             ; R1 = Pop colour
                LD R4, curr_led         ; R4 = Pointer to current LED
                LD R5, [R4]             ; R5 = Current LED
                ST R1, [R5]             ; Write green/red to square
                ADD R5, R5, #1
                ST R5, [R4]             ; Update current LED

            update_score:
                SUB R6, R6, #1
                LD R1, [R6]             ; R1 = Pop score
                CMP R1, R0
                BEQ exit_is             ; No score update for incorrect answer

                LD R4, score_rewrite    ; R4 = Pointer to '00'
                ADD R4, R4, #1              
                LD R5, [R4]             ; R5 = Score second digit
                LD R1, ascii_score
                SUB R5, R5, R1          ; Ascii to number
                CMP R5, #9
                BEQ update_tens

                LD R5, [R4]                 
                ADD R5, R5, #1
                ST R5, [R4]             ; +1 to units column
                B exit_is

            update_tens:
                ST R1, [R4]             ; 0 in units columns
                SUB R4, R4, #1
                LD R1, [R4]              
                ADD R1, R1, #1
                ST R1, [R4]             ; +1 to tens columns

            exit_is:
                SUB R6, R6, #1      ; Pop return address to PC
                LD PC, [R6]

        ORG 0x0140
        B submit
        ;-----> Function: Reading & checking answer submission
        ;          param: _
        ;         return: _ 
        ascii_read DEFW 48
        submit:
            SUB R6, R6, #1              ; 'pop' to remove unneeded parameter
            LD R5, [R0, #lcd_table]
            ADD R5, R5, #5              ; R5 = Pointer to input_max
            LD R5, [R5]
            find_units:
                LD R4, [R5]
                CMP R4, R0              ; Move R4 to contain units column
                BNE exit_u
                SUB R5, R5, #1
                B find_units
            exit_u:
                LD R3, ascii_read
                SUB R4, R4, R3          ; Ascii to num
                MOV R1, R4              ; Initialise sum with units column
                
                SUB R5, R5, #1
                LD R4, [R5]
                CMP R4, R0              ; Check for tens column
                BEQ exit_submit         ;   and skip if not present
                SUB R4, R4, R3          ; Ascii to num
            
            MOV R2, #10
            find_tens:
                CMP R2, R0
                BEQ exit_ft
                ADD R1, R1, R4          ; Add 'tens' to sum 10 times
                SUB R2, R2, #1
                B find_tens
            exit_ft:
                SUB R5, R5, #1
                LD R4, [R5]
                CMP R4, R0              ; Check for hundreds column
                BEQ exit_submit            ; and skip if not present
                SUB R4, R4, R3          ; Ascii to num
            
            MOV R2, #10
            find_hundreds:
                CMP R2, R0
                BEQ exit_submit
                MOV R3, #10
                find_hundreds_inner:
                    CMP R3, R0
                    BEQ exit_fhi
                    ADD R1, R1, R4
                    SUB R3, R3, #1
                    B find_hundreds_inner
                exit_fhi:
                SUB R2, R2, #1
                B find_hundreds
        
            exit_submit:
                SUB R6, R6, #1
                LD R2, [R6]             ; R2 = return address 
                SUB R6, R6, #1
                LD R3, [R6]             ; R3 = Expected answer
                CMP R1, R3              
                BNE incorrect           ; R1 = User answer

            correct:
                ADD R1, PC, #10
                ST R1, [R6]             ; Push return address
                ADD R6, R6, #1
                MOV R5, #1
                ST R5, [R6]             ; Push '1' for score increment
                ADD R6, R6, #1
                LD R5, green
                ST R5, [R6]             ; Push green for LED increment
                ADD R6, R6, #1
                LD R4, [R0, #branch_table]
                LD PC, [R4, #3]         ; B increment_score
            B exit_s

            green DATA 0b0000000000011100
            red DATA 0b0000000011100000

            incorrect:
                ADD R1, PC, #10
                ST R1, [R6]             ; Push return address
                ADD R6, R6, #1
                MOV R5, #0
                ST R5, [R6]             ; Push '0' for score increment
                ADD R6, R6, #1
                LD R5, red
                ST R5, [R6]             ; Push red for LED increment
                ADD R6, R6, #1
                LD R4, [R0, #branch_table]
                LD PC, [R4, #3]         ; B increment_score

            exit_s:
                ;MOV PC, R2              ; Pop return address to PC
                LD R2, [R0, #branch_table]
                LD PC, [R2, #4]          ; B generate_question

        ORG 0x01B0
        update_answer:
            CMP R3, #10
            ADD R1, PC, #9
            ST R1, [R6]
            ADD R6, R6, #1              ; Push return address
            ST R3, [R6]
            ADD R6, R6, #1              ; Push keypad input
            BGT skip_rewritelcd

            LD R4, [R0, #branch_table]
            LD PC, [R4, #1]             ; B rewrite_lcd using branch table

            skip_rewritelcd:
                LD R4, [R0, #branch_table]
                LD PC, [R4, #2]             ; B submit using branch table

                LD PC, [PC]
                DEFW 0x00C0                 ; Retake next input if no submit (FIX?!)
        
    end_game:

stop
B stop



; ============================= Stack & Branch Table ==============================
Stack:
    ORG 0x800

Branch_table:
    ORG 0x900
    Update_answer DATA 0x01B0
    Rewrite_lcd DATA 0x00E0
    Submit DATA 0x0140
    Increment_score DATA 0x0115
    Question_generation DATA 0x0055


; ================================ Storing Strings ================================
String_table:
    ORG 0x1000
    start_menu DATA 0x1050
    score DATA 0x1100

    start_menu_string:
        ORG 0x1050; PRESS * TO START
        DEFW 0x50
        DEFW 0x52
        DEFW 0x45
        DEFW 0x53
        DEFW 0x53
        DEFW 0x20
        DEFW 0x2A
        DEFW 0x20
        DEFW 0x54
        DEFW 0x4F
        DEFW 0x20
        DEFW 0x53
        DEFW 0x54
        DEFW 0x41
        DEFW 0x52
        DEFW 0x54
        DEFW 0x21
        DEFW 0

    score_string:
        ORG 0x1100; Score: 00
        DEFW 0x53
        DEFW 0x43
        DEFW 0x4F
        DEFW 0x52
        DEFW 0x45
        DEFW 0x3A
        DEFW 0x20
        DEFW 0x30       ; 0x1107
        DEFW 0x30       ; 0x1108
        DEFW 0



; ================================ Variable Tables ================================
Tables:
    LED:
        ORG 0x1200
        led_start DATA 0xFF00
        led_end DATA 0xFF3F
        led_pointer DATA 0xFF00

    LCD:
        ORG 0x1210
        lcd_start DATA 0xFF40
        lcd_end DATA 0xFF8F
        score_pointer DATA 0xFF68
        input_pointer DATA 0xFF7C
        input_min DATA 0xFF7C
        input_max DATA 0xFF7F
        Score_rewrite DATA 0xFF6F
        question_end DATA 0xFF46

    Keypad:
        ORG 0x1220
        keypad DATA 0xFF94
        asterisk DATA 0b0000010000000000
        hashtag DATA 0b0000100000000000