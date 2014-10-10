; -----------------
; kernek.s
;------------------

;extern kernel_entry     ; int kernel_entry()

[section .text]

global _start

_start:

    mov ah, 0Fh
    mov al, 'K'
    mov [gs:((80 * 1 + 39) * 2)], ax

    jmp $
    ;call kernel_entry

;times 10240-($-$$) db 0
