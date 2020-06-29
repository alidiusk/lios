; receiving data in dx
print_hex:
    pusha

    mov cx, 0 ; index variable

; convert to ascii, 0x30 - 0x39 are numbers, 0x41 - 0x46 are 'A' - 'F'
hex_loop:
    cmp cx, 4
    je end

    ; convert last char of dx to ascii
    mov ax, dx
    and ax, 0x000f ; mask first three to zeroes
    add al, 0x30
    cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
    jle step2
    add al, 7 ; 'A' is ascii 65 instead of 58, so 65 - 58 = 7

step2:
    ; get correct position of string to place our ascii character
    ; bx <- base address + string length - index
    mov bx, HEX_OUT + 5
    sub bx, cx
    mov [bx], al ; copy ascii character from 'al' to position pointed to by 'bx'
    ror dx, 4 ; rotate through

    add cx, 1
    jmp hex_loop

end:
    ; prepare parameter and call the function
    ; print receives parameters in 'bx'
    mov bx, HEX_OUT
    call print

    popa
    ret

HEX_OUT:
    db '0x0000', 0 ; reserved memory for our new string
