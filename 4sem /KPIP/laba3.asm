.model small
.stack 100h

.data
     message1 db 10, 13, "Enter first number:$"
     message2 db 10, 13, "Enter second number:$"
     message3 db 10, 13, "Result number:$"
     enter    db 10, 13, "$"
     
     menu_message1 db 10, 13, "Menu:$"
     menu_message2 db 10, 13, "1)Operation AND$"
     menu_message3 db 10, 13, "2)Operation OR$"
     menu_message4 db 10, 13, "3)Operation XOR$"
     menu_message5 db 10, 13, "0)Exit program$"  
     
     menu2_message1 db 10, 13, "Menu2$"
     menu2_message2 db 10, 13, "1)Input new numbers$"
     menu2_message3 db 10, 13, "0)Exit the program$"
     
     error1 db 10, 13, "You don't enter something!$"
     error2 db 10, 13, "You are out of the range!$"
     error3 db 10, 13, "You entered incorrect symbols!$"
     
     min_number db "-32768$"
     max_number db "32767$'"
          
     length equ 10
     
     choise1 db length, length dup(?)
     choise2 db length, length dup(?)
     
     Strnumber1  db length, length dup(?) 
     Strnumber2  db length, length dup(?)         
     Strnumber3  db length, length dup(?)
     
.code
output macro str
    lea dx, str
    mov ah, 09h
    int 21h
endm 


input macro str
    lea dx, str
    mov ah, 0Ah
    int 21h
endm


atoi macro str
    local atoi_cycle
    local atoi_negative
    local atoi_return
     
    push bx
    mov dx, 1
    xor cx, cx
    xor bx, bx
    mov cl, str[1]
    lea si, str
    add si, 1
    add si, cx
    std

atoi_cycle:
    xor ax, ax
    lodsb
    
    cmp al, '-'
    je atoi_negative
    
    sub al, 48
    push dx
    mul dx
    pop dx
    add bx, ax
    mov ax, dx
    mov dx, 10
    mul dx
    mov dx, ax
    dec cl
    jnz atoi_cycle
    jmp atoi_return

atoi_negative:
    neg bx

atoi_return:
    mov ax, bx
    pop bx
endm    


itoa macro str
    xor bx, bx
    lea bx, str
    add bx, 2
    
    xor cx, cx
    cmp ax, 0
    jge itoa_cycle
    
    mov [bx], '-'
    inc bx
    neg ax

itoa_cycle:
    cmp ax, 10
    jl itoa_end
    xor dx, dx
    push bx
    mov bx, 10
    div bx
    pop bx
    add dl, 48
    push dx
    inc cx
    jmp itoa_cycle

itoa_end:
    mov [bx], al
    add [bx], 48
    inc bx

itoa_fill_string:
    cmp cx, 0
    je itoa_return
    pop dx
    mov [bx], dl
    inc bx
    dec cx
    jmp itoa_fill_string

itoa_return:
    mov [bx], '$'
endm


check_on_input macro str 
    local check_positive
    local check_correct
    local check
    local check_end
    local check_error1
    local check_error2
    local check_error3
    local check_return
    
    xor bl, bl
    xor ch, ch
    mov cl, str[1]
    cmp cl, 0
    je check_error1
    
    cld
    lea si, str
    add si, 2
    
    cmp str[2], '-'
    jne check_positive
    
    inc si
    dec cl
    cmp cl, 5
    jne check
    lea di, min_number
    inc di
    mov bl, 1 
    jmp check

check_positive:
    cmp cl, 6 
    jl check_correct
    cmp str[2], '0'
    jne check_error2
    dec cl
    inc si
check_correct:
    cmp cl, 5
    jl check
    mov bl, 1
    lea di, max_number

check:
    lodsb
    sub al, 48
    cmp al, 0
    jl check_error3
    cmp al, 9
    jg check_error3
    cmp bl, 1
    jne check_end

    mov ah, al
    xchg si, di
    lodsb
    sub al, 48
    xchg si, di
    cmp ah, al
    jg check_error2
    je check_end
    xor bl, bl

check_end:
    dec cl
    jnz check
    xor ax, ax
    jmp check_return

check_error1:
    output error1 
    mov ax, 1h
    jmp check_return
check_error2:
    output error2 
    mov ax, 1h
    jmp check_return
check_error3:
    output error3
    mov ax, 1h

check_return:
endm


main:
    mov ax, @data
    mov ds, ax 
    
Input:    
    output message1
    output enter
    input StrNumber1
    xor bx, bx
    mov bl, StrNumber1[1]
    mov StrNumber1[bx+2], '$'
    output enter
    check_on_input StrNumber1
    cmp ax, 1
    je Input

Input2:    
    output message2
    output enter
    input StrNumber2
    xor bx, bx
    mov bl, StrNumber2[1]
    mov StrNumber2[bx+2], '$'
    output enter    
    check_on_input StrNumber2
    cmp ax, 1
    je Input2
    
    atoi StrNumber1  
    mov bx, ax 
    push bx
    
    atoi StrNumber2 
    push ax

Menu:  
    call Output_Menu 
    xor dx, dx
    lea dx, choise1    
    mov ah, 0Ah
    int 21h
    output enter
    atoi choise1
    
    cmp ax, 0
    je Exit
    cmp ax, 1
    je Operation_AND
    cmp ax, 2
    je Operation_OR   
    cmp ax, 3
    je Operation_XOR 
    jne Menu

Operation_AND:
    pop ax 
    pop bx
    and bx, ax
    jmp toString              
    
Operation_OR:
    pop ax
    pop bx
    or bx, ax
    jmp toString
    
Operation_XOR:
    pop ax
    pop bx
    xor bx, ax
    jmp toString
    
toString:
    xor ax, ax
    mov ax, bx
    itoa StrNumber3
    output message3
    output enter 
    lea dx, StrNumber3+2
    call Output_String 
    output enter  
    
Menu2:  
    call Output_Menu2
    xor dx, dx
    lea dx, choise2    
    mov ah, 0Ah
    int 21h
    output enter
    atoi choise2
    
    cmp ax, 0
    je Exit
    cmp ax, 1
    je Input
    jne Menu2
    
    
Output_Menu proc
    output menu_message1
    output menu_message2
    output menu_message3
    output menu_message4
    output menu_message5
    output enter 
    ret
Output_Menu endp


Output_Menu2 proc
    output menu2_message1
    output menu2_message2
    output menu2_message3
    output enter
    ret
Output_Menu2 endp
    

Output_String proc
    mov ah, 09h
    int 21h
    ret
Output_String endp 
          

Exit:       
    mov ax, 4c00h
    int 21h