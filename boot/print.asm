print:
    pusha

start:
    mov al, [bx] ; base address for string
    cmp al, 0
    je done

    ; print with bios help
    mov ah, 0x0e
    int 0x10 ; al already contains character

    add bx, 1 ; continue to next char
    jmp start

done:
    popa
    ret

print_nl:
    pusha

    mov ah, 0x0e
    mov al, 0x0a ; newline character
    int 0x10
    mov al, 0X0d ; carriage return
    int 0x10

    popa
    ret
