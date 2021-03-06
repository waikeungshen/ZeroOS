; -----------------
; kernek.s
;------------------

extern kernel_entry     ; int kernel_entry()

[section .bss]
StackSpace  resb    2 * 1024    ; 堆栈大小2K
StackTop:                       ; 栈顶

[section .text]

global _start   ; 导出 _start

_start:
    ; 切换堆栈
    mov esp, StackTop

    mov ah, 0Fh
    mov al, 'K'
    mov [gs:((80 * 1 + 39) * 2)], ax

    call kernel_entry

    hlt
    ;jmp $
;times 10240-($-$$) db 0
