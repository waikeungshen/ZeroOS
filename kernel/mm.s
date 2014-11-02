global load_cr0
global store_cr0

load_cr0:   ; u32 load_cr0();
    mov eax, cr0
    ret

store_cr0:  ; void store_cr0(u32);
    mov eax, [esp+4]
    mov cr0, eax
    ret
