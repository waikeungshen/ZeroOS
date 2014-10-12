global idt_flush

idt_flush:
    mov eax, [esp+4]    ; 参数存入 eax 寄存器
    lidt [eax]          ; 加载 IDTR
    ret

;--------------------------------------------------
; 原本代码量大，所以这里采用宏指令
; 用于没有错误代码的中断
; 声明宏指令 ISR_NOERRCODE
%macro ISR_NOERRCODE 1
global isr%1                ; 导出函数，在include/idt.h 中定义的函数
isr%1:
    cli                     ; 关闭中断
    push 0                  ; 没有错误代码，所以用0来代替，占位
    push %1                 ; push 中断号
    jmp isr_common_stub     ; 完成剩下的工作
%endmacro

; 用于有错误代码的中断
; 声明宏指令 ISR_ERRCODE
%macro ISR_ERRCODE 1
global isr%1                ; 导出函数，在include/idt.h 中定义的函数
isr%1:
    cli                     ; 关闭中断
    push %1                 ; push 中断号
    jmp isr_common_stub     ; 完成剩下的工作
%endmacro
;---------------------------------------------------

; 由宏指令完成定义中断处理函数，即void isr1() ~ void isr255()
; 0 ~ 19 CPU异常
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
; 20 ~ 31 Intel保留
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
; 32 ~ 255 用户自定义
ISR_NOERRCODE 255

global isr_common_stub  ; 导出中断服务程序

extern isr_handler ; 导入异常处理函数

; 中断服务程序
isr_common_stub:
    pusha       ; push edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds
    push eax    ; 保存数据段描述符

    mov ax, 0x10    ; 加载内核数据段描述符
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push esp
    call isr_handler
    add esp, 4

    pop eax     ; 恢复原来的数据段描述符
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    popa        ; pops edi, esi, ebp, esp, ebx, edx, ecx, eax
    add esp, 8  ; 清理栈中的error code 和 ISP
    iret
