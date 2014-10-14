global gdt_flush

gdt_flush:
    mov eax, [esp+4]    ; 参数存入 eax 寄存器
    lgdt [eax]          ; 加载 GDTR

    mov ax, 0x10        ; 加载数据段描述符
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; 0x08 是当前代码段描述符
.flush:
    ret
