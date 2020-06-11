.model small       
.stack 100h

.data
    menuMess db "                                  Control:", 0Ah ,0Dh
             db "                              Up - spin figure", 0Ah ,0Dh
             db "                       Down - move figure down faster", 0Ah, 0Dh
             db "                    Left/Right - move figure to left/right", 0Ah, 0Ah, 0Ah, 0Ah, 0Ah, 0Ah, 0Ah, 0Dh
             db "                                   Menu:", 0Ah, 0Dh
             db "                               Enter - start", 0Ah, 0Dh
             db "                                 Esc - exit",0Dh ,0Ah,'$'
     
     scoreMess db 09h, 09h, 09h, 09h, "Score: $"                       
     
     score dw 0
     
     fieldSize equ 216
     field db fieldSize dup(0)
     colors db fieldSize dup(0)
     
     lines equ 18
     coloumns equ 12
     
     Figures:
        db 0, 1, 1
        db 0, 1, 1
        db 0, 0, 0
        
        db 1, 1, 1
        db 0, 0, 1
        db 0, 0, 1
        
        db 0, 1, 0
        db 0, 1, 0
        db 0, 1, 0
        
        db 1, 1, 1
        db 0, 0, 1
        db 0, 0, 0
        
        db 1, 1, 1
        db 0, 1, 0
        db 0, 0, 0
        
        db 0, 1, 0
        db 0, 1, 1
        db 0, 0, 1
     
     color db ?
     coffset db ?
        
     figure db 9 dup(0)
     figureBuf db 9 dup(0)
     buffer db 9 dup(0)
     
     figX dw 0
     figY dw 0
     
     poffset equ 2
     ;myoffset dw ?
     
     delay dw 18
     timer dw ?
          
     scoreOffset1 dw 80
     scoreOffset2 dw 1844
     
.code 
output macro str 
    mov ah, 09h
    lea dx, str
    int 21h
endm 


outputScore macro offset
    local pushStack
    local outputStack
     
    pusha
    mov ax, score
    
    xor cx, cx
    xor dx, dx
    
    mov si, 10
pushStack:
    div si
    add dl, '0'
    push dx
    xor dx, dx
    inc cx
    cmp ax, 0
    jne pushStack
    
    mov bx, word ptr offset
    
outputStack:
    pop dx
    push ds
    
    mov ax, 0B800h
    mov ds, ax
    
    mov [bx], dl
    inc bx
    mov [bx], 0Ch
    inc bx
    
    pop ds
    loop outputStack
    
    call hideCursor
    
    popa
endm


nullify macro mass
    push ax
    push di
    
    cld
    lea di, [mass]
    mov cx, fieldSize
    xor al, al
    rep stosb
    
    pop di
    pop ax
endm


start: 
    mov ax, @data
    mov ds, ax 
    mov ax, @data
    mov es, ax 

    call clearScreen 
    call startScreen

GameStart:
    nullify field
    nullify colors
    call nullifyScore
    call setDelay
        
    call clearScreen    
    call startGame
    call createFigure
    call putFigure
    jmp Game
    
NewFigure:
    call createFigure
    call checkPutFigure
    cmp ax, 0
    jne loseMark
    call putFigure
    
Game:
    call displayField
    call outputBorder
    call getTime
    add ax, word ptr delay
    mov word ptr timer, ax
    
cycle:
    call displayField
    call outputBorder
    jmp checkTime
    
cycle1:
    mov ah, 01h
    int 16h
    jz  cycle
    
    mov ah, 00h
    int 16h

    cmp ah, 01h
    je loseMark
    
fasterDown:
    cmp ah, 21h
    jne slowerDown
    
    cmp delay, 3
    jle Game
    sub delay, 3
    
slowerDown:
    cmp ah, 1Fh
    jne checkMove
    
    cmp delay, 54
    jge Game
    add delay, 3
    
checkMove:
    lea si, figure
    cmp ah, 04Bh
    jne mark1
    
leftMark:
    call clearFigure
    
    mov ax, figX
    cmp ax, 0
    je tryMoveLeft
    
    sub ax, 1
    mov figX, ax
    call checkPutFigure
    cmp ax, 0
    jne cantLeft
    
    call putFigure
    
    jmp cycle
       
tryMoveLeft:
    lea si, figure
    lea di, figureBuf
    
    push cx
    mov cx, 9
    call figcpy
    pop cx
    
    call moveLeft
    call checkPutFigure
    cmp ax, 0
    je moveLeftOnce
    
    lea si, figureBuf
    lea di, figure
    push cx
    mov cx, 9
    call figcpy
    pop cx
    
moveLeftOnce:
    call putFigure
    jmp cycle

cantLeft:
    mov ax, figX
    add ax, 1
    mov figX, ax
    
    call putFigure
    
    jmp cycle


mark1:
    cmp ah, 04Dh
    jne mark2
    
rightMark:
    call clearFigure
    
    mov ax, coloumns
    sub ax, 3
    mov bx, ax
    mov cx, ax
    add cx, 1
    
    mov ax, figX
    add ax, 1
    mov figX, ax
    cmp ax, cx
    je tryMoveRight
    cmp ax, bx
    ja cantRight
    
    call checkPutFigure
    cmp ax, 0
    jne cantRight
    
    call putFigure
    
    jmp cycle
    
tryMoveRight:
    mov ax, figX
    sub ax, 1
    mov figX, ax
    
    lea si, figure
    lea di, figureBuf
    
    push cx
    mov cx, 9
    call figcpy
    pop cx
    
    call moveRight
    call checkPutFigure
    
    cmp ax, 0
    je moveRightOnce
    
    lea si, figureBuf
    lea di, figure
    
    push cx
    mov cx, 9
    call figcpy
    pop cx
    
moveRightOnce:
    call putFigure
    jmp cycle
    
cantRight:
    mov ax, figX
    sub ax, 1
    mov figX, ax
    
    call putFigure
    
    jmp cycle
        
    
mark2:
    cmp  ah, 050h
    jne mark3
    
downMark:
    call clearFigure
    
    mov ax, lines
    sub ax, 3
    mov bx, ax
    mov cx, ax
    add cx, 1 
    
    mov ax, figY
    add ax, 1
    mov figY, ax
    cmp ax, cx
    je tryMoveDown
    cmp ax, bx
    ja cantDown
    
    call checkPutFigure
    cmp ax, 0
    jne cantDown
    
    call putFigure
    
    jmp Game
    
tryMoveDown:
    mov ax, figY
    sub ax, 1
    mov figY, ax
    
    lea si, figure
    lea di, figureBuf
    
    push cx
    mov cx, 9
    call figcpy
    pop cx
    
    call moveDown
    call checkPutFigure
    
    cmp ax, 0
    je moveDownOnce
    
    lea si, figureBuf
    lea di, figure
    
    push cx
    mov cx, 9
    call figcpy
    pop cx
    
moveDownOnce:
    call putFigure
    call checkLines
    jmp NewFigure
    
cantDown:
    mov ax, figY
    sub ax, 1
    mov figY, ax
    
    call putFigure
    call checkLines
    
    jmp NewFigure    


mark3:
    cmp ah, 048h
    jne checkTime
    
spinMark:
    call clearFigure
    
    lea si, figure
    lea di, figureBuf
    
    push cx
    mov cx, 9
    call figcpy
    pop cx
    
    call spinFigure
    call checkPutFigure
    
    cmp ax, 0
    jne cantSpin
    
    call putFigure
    
    jmp cycle
    
cantSpin:
    lea si, figureBuf
    lea di, figure
    
    push cx
    mov cx, 9
    call figcpy
    pop cx
    
    call putFigure
    
    jmp cycle

    
checkTime:
    call getTime
    cmp ax, timer
    jl cycle1
    jmp downMark
    
     
loseMark: 
    call clearScreen
    
    call hideCursor
    call outputLoseMess
    call outputLoseScore
    call outputPressKey
   
    
waitKey:
    mov ah, 01h
    int 16h    
    
    jz waitKey
    xor ah, ah
    int 16h
    ;cmp ah, 1Ch
    ;je enterWelcome
    ;cmp ah, 01h
    ;jne waitKey
    
    call clearScreen
    call startScreen    
    cmp ah, 1Ch
    je GameStart
    
    
exit:    
    mov ax, 4C00h
    int 21h
    
clearScreen proc
    mov ax, 0003h
    int 10h 
    ret
clearScreen endp
 
 
startScreen proc
    ;push ax
    push bx
    push dx
    push ds
    
    mov ax, 0B800h
    mov ds, ax
    
    pop ds
    
    mov ah, 09h
    mov al, ' '
    xor bh, bh
    mov bl, 0Ah
    mov cx, 1500
    int 10h
    
    output menuMess 
    call hideCursor
    
waitEnter:
    mov ah, 01h
    int 16h    
    
    jz waitEnter
    xor ah, ah
    int 16h
    cmp ah, 1Ch
    je enterWelcome
    cmp ah, 01h
    jne waitEnter
    call clearScreen
    
    mov ax, 4C00h
    int 21h
    
enterWelcome:
    pop dx
    pop bx
    ;pop ax
    
    ret
startScreen endp  
 

hideCursor proc
    mov ah, 02h
    mov dh, 26   ;row number
    xor dl, dl   ;column number
    int 10h
    ret
hideCursor endp


startGame proc
     output scoreMess
     
     outputScore scoreOffset1
     
     xor ax, ax
     mov cx, fieldSize
     call outputBorder
     
     ret 
startGame endp
 
 
outputBorder proc
    push bx
    push es
    
    mov ax, 0B800h
    mov es, ax
    mov di, 0
    
    mov cx, lines
vertical: 
    mov es:[di], 0DBh
    add di, 1
    mov es:[di], 0Fh
    add di, 1
    mov ax, coloumns
    add ax, ax
    mov bx, ax
    add di, bx
    
    mov es:[di], 0DBh
    add di, 1
    mov es:[di], 0Fh
    mov ax, 160
    sub ax, bx
    add di, ax
    sub di, 3
    loop vertical
    
    mov cx, coloumns
    add cx, 2
horizontal:
    mov es:[di], 0DBh
    add di, 1
    mov es:[di], 0Fh
    add di, 1
    loop horizontal
    
    pop es 
    pop bx
    
    ret
outputBorder endp
  
  
createFigure proc
    call randNum
    
    mov bx, 9
    mul bx
    add ax, offset Figures
    mov si, ax
    mov di, offset figure
    
    mov cx, 9
    call figcpy 
    
    xor ax, ax
    mov figX, ax
    mov figY, ax
    
    call randNum
    mov color, al
    add color, 09h    
        
    ret
createFigure endp


randNum proc
    push bx
    push cx
    
rloop:
    xor bx, bx
    
    mov ah, 2Ch ;get time
    int 21h
    
    mov bl, dl
    mov ah, 00h ;tic counter
    int 1Ah
    
    mov ax, dx
    mul bx
    mov bx, 10
    mov al, dl
    
    xor dx, dx
    div bx
    
    xor ax, ax
    mov al, dl
    cmp al, 06h
    jae rloop
    
    pop cx
    pop bx
    
    ret
randNum endp 
 
 
figcpy proc
    cld
    rep movsb
    
    ret 
figcpy endp    


putFigure proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    lea di, field
    
    mov ax, figY
    mov bx, coloumns
    mul bx
    add ax, figX
    add di, ax
    
    mov cx, 3
    coloumnloop:
        push cx
        
        mov cx, 3
        linesloop:
            mov al, [si]
            cmp al, 0
            je zero1
            mov es:[di], al
            
            push di
            mov ax, di
            sub ax, offset field
            lea di, colors
            add di, ax
            mov al, color
            mov es:[di], al
            pop di
            
        zero1:
            inc si
            inc di
            
            loop linesloop
            
        pop cx
        mov ax, coloumns
        sub ax, 3
        add di, ax
        
        loop coloumnloop
        
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    
    ret
putFigure endp
 
 
displayField proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push ds
    
    mov ax, 0B800h
    mov es, ax
    mov di, poffset
    lea si, field
    mov cx, lines
    
    displayColoumn:
        push cx
        mov cx, coloumns
        
        displayLine:
            mov al, [si]
            cmp al, 0
            jne one
        zero2:
            mov al, ' '
            mov es:[di], al
            ;add di, 1
            jmp displayNext
        one:
            mov al, 0DBh
            mov es:[di], al
            add di, 1 
            ;mov al, color
            ;lea ax, colors
            ;add al, coffset
            ;mov es:[di], al
            
            push si
            mov ax, si
            sub ax, offset field
            lea si, colors
            add si, ax
            mov al, [si]
            mov es:[di], al
            pop si
            
            sub di, 1
            jmp displayNext
        
        displayNext:
            add di, 2
            inc si
                     
            loop displayLine
            
        pop cx
        mov ax, coloumns
            
        mov bx, 2
        mul bx
        mov bx, ax
        mov ax, 160
        sub ax, bx
        add di, ax 
        loop displayColoumn
        
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
        
    ret            
displayField endp


getTime proc
    push ds
    
    xor ax, ax
    mov ds, ax
    mov ax, word ptr ds:[46ch]
    
    pop ds
    
    ret
getTime endp


clearFigure proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    lea di, field
    
    mov ax, figY
    mov bx, coloumns
    mul bx
    add ax, figX
    add di, ax
    
    mov cx, 3
    clearColoumns:
        push cx
        
        mov cx, 3
        clearLines:
            mov al, [si]
            cmp al, 0
            je clearContinue
            mov [di], 0
            
        clearContinue:
            inc si
            inc di
            
            loop clearLines
        
        pop cx
        mov ax, coloumns
        sub ax, 3
        add di, ax
        
        loop clearColoumns
        
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
clearFigure endp              


checkPutFigure proc
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    lea di, field
    
    mov ax, figY
    mov bx, coloumns
    mul bx
    add ax, figX
    add di, ax
    
    mov cx, 3
    checkPutColoumns:
        push cx
        
        mov cx, 3
        checkPutLines:
            mov al, [si]
            mov ah, [di]
            cmp al, 0
            je zero3
            
            mov al, [di]
            cmp al, 1
            je checkPutError
            
        zero3:
            inc si
            inc di
            
            loop checkPutLines
            
        pop cx
        mov ax, coloumns
        sub ax, 3
        add di, ax
        
        loop checkPutColoumns
        
    xor ax, ax
    jmp checkPutReturn
    
checkPutError:
    pop cx
    xor ax, ax
    add ax, 1
    
checkPutReturn:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    ret
checkPutFigure endp


moveLeft proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    add si, 0
    
    mov cx, 3
    checkLeft:
        mov al, [si]
        cmp al, 0
        jne cantMoveLeft
        
        add si, 3
        loop checkLeft
        
    lea di, figure
    mov bx, 0
    mov cx, 3
    leftLoop:
        mov al, [di+bx+1]
        mov [di+bx], al
        
        mov al, [di+bx+2]
        mov [di+bx+1], al
        
        mov al, 0
        mov [di+bx+2], al
        
        add bx, 3
        loop leftLoop
        
cantMoveLeft:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
moveLeft endp


moveRight proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    add si, 2
    
    mov cx, 3
    checkRight:
        mov al, [si]
        cmp al, 0
        jne cantMoveLeft
        add si, 3
        loop checkRight
    
    lea di, figure
    mov bx, 0
    mov cx, 3
    rightMove:
        mov al, [di+bx+1]
        mov ]di+bx+2], al
        
        mov al, [di+bx]
        mov [di+bx+1], al
        
        mov al, 0
        mov [di+bx], al
        
        add bx, 3
        loop rightMove
        
cantMoveRight:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
moveRight endp


moveDown proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea si, figure
    add si, 6
    
    mov cx, 3
    checkDown:
        mov al, [si]
        cmp al, 0
        jne cantMoveDown
        
        inc si
        loop checkDown
        
    lea di, figure
    mov bx, 0
    mov cx, 3
    downMove:
        mov al, [di+bx+3]
        mov [di+bx+6], al
        
        mov al, [di+bx]
        mov [di+bx+3], al
        
        mov al, 0
        mov [di+bx], al
        
        add bx, 1
        loop downMove
        
cantMoveDown:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
moveDown endp


spinFigure proc
    push cx
    
    lea si, figure
    lea di, buffer
    mov cx, 9
    mov bx, 6
    
    spin:
        push si
                   
        add si, bx
        sub bx, 3
        cmp bx, 0
        jge pos   
        add bx, 10 
    
    pos:
        mov al, [si]
        mov [di], al
        inc di
        pop si 
        loop spin
        
    lea si, buffer
    lea di, figure
    mov cx, 9
    call figcpy
    
    pop cx
    ret
spinFigure endp


checkLines proc   
; bx -- curr row number
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    lea di, field
    mov bx, 0
    
lineLoopCheck:    
    mov cx, coloumns
coloumnLoopCheck: 
    mov al, [di]
    cmp al, 0
    je nextLine
    inc di
    loop coloumnLoopCheck
    dec di 
moveAll:
    push di
             
    cmp bx, 0
    je  clearFirstLine              
                    
    mov ax, coloumns
    mul bx
    sub ax, 1
    lea si, field
    add si, ax
    
    push cx
    mov cx, ax     
    ;because ax -- is index of the element
    ;and cx -- is current amount of them
    add cx, 1        
    std
    
    push di
    push cx
        
    rep movsb
    ;mov dx, di
    
    pop cx
    pop di
    ;push si
    
    sub di, offset field
    add di, offset colors
    
    lea si, colors
    add si, ax
    
    std
    rep movsb
    
    
clearFirstLine:
    lea di, field  
    mov cx, coloumns
    mov al, 0
    cld
    rep stosb
    
    push di
    
    lea di, colors
    mov cx, coloumns
    mov al, 0
    cld
    rep stosb
    
    pop di
    
    add score, 100
    outputScore scoreOffset1    
    
    pop cx
    pop di
    
nextLine:
    add bx, 1
    cmp bx, lines
    je exitCheck
    
    mov ax, bx
    mov cx, coloumns
    mul cx
    lea di, field
    add di, ax
   
    jmp lineLoopCheck  
    
exitCheck:        
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
checkLines endp


outputLoseMess proc   
    push ds
    
    mov ax, 0b800h
    mov ds, ax
    
    mov bx, 1670;distance                            
    mov [bx], 'Y'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'o'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'u'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], ' '
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'l'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'o'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 's'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'e'
    inc bx
    mov [bx], 0Fh                  
   
    pop ds
    
    ret     
outputLoseMess endp


outputLoseScore proc   
    push ds
    
    mov ax, 0b800h
    mov ds, ax
    
    mov bx, 1828;distance                            
    mov [bx], 'S'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'c'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'o'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'r'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'e'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], ':'
    inc bx
    mov [bx], 0Fh
      
    pop ds
    
    outputScore scoreOffset2
    
    ret    
outputLoseScore endp


outputPressKey proc   
    push ds
    
    mov ax, 0b800h
    mov ds, ax
    
    mov bx, 1986;distance                            
    mov [bx], 'P'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'r'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'e'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 's'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 's'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], ' '
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'a'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'n'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'y'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], ' '
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'k'
    inc bx 
    mov [bx], 0Fh
    inc bx
    mov [bx], 'e'
    inc bx
    mov [bx], 0Fh
    inc bx
    mov [bx], 'y'
    inc bx
    mov [bx], 0Fh     
    
    pop ds
    
    ret    
outputPressKey endp


nullifyScore proc
    push ax
    
    xor ax, ax
    mov score, ax
    
    pop ax
    
    ret
nullifyScore endp


setDelay proc
    push ax
    
    xor ax, ax
    mov ax, 18
    mov delay, ax
    
    pop ax
       
    ret
setDelay endp