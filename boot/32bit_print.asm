; always prints to the top of the screen
[bits 32] ; use 32bit protected mode

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f ; color byte for each character

; takes address of the string in 'ebx'
print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY

print_string_pm_loop:
    mov al, [ebx] ; [ebx] is the address of our character
    mov ah, WHITE_ON_BLACK

    cmp al, 0 ; end of string
    je print_string_pm_done

    mov [edx], ax ; store character + attribute in video memory
    add ebx, 1 ; next character
    add edx, 2 ; next memory position

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret
