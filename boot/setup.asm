org 0100h

BaseOfSetup         equ         09000h ; Setup.bin 被加载到的位置 --- 段地址
OffsetOfSetup       equ         0100h  ; setup.bin 被加载到的位置 --- 偏移地址
BaseOfStack         equ         0100h
BaseOfKernelFile    equ         08000h ; kernel.bin 被加载到的位置 --- 段地址
OffsetOfKernelFile  equ         0h     ; kernel.bin 被加载到的位置 --- 偏移地址
BaseOfSetupPhyAddr  equ         BaseOfSetup*10h    ; Setup.bin 被加载到的位置 --- 物理地址
BaseOfKernelFilePhyAddr equ     BaseOfKernelFile*10h    ; kernel.bin 被加载到的位置 --- 物理地址

PageDirBase         equ     100000h             ; 页目录开始地址 1M
PageTblBase         equ     101000h             ; 页表开始地址   1M+4K

KernelEntryPointPhyAddr     equ     0x30000     ; 

jmp start

; GDT
;                   
LABEL_GDT:	        ; 空描述符      
    dw  0x0000
    dw  0x0000
    dw  0x0000
    dw  0x0000
LABEL_DESC_FLAT_C:  
    dw  0x07FF      ; 2048*4K = 8Mb
    dw  0x0000      ; base address = 0
    dw  0x9A00      ; code read/exec
    dw  0x00C0      ; granularity=4096, 386
LABEL_DESC_FLAT_RW: 
    dw  0x07FF
    dw  0x0000
    dw  0x9200      ; code read/write
    dw  0x00C0
LABEL_DESC_VIDEO:   
    dw  0xFFFF      ; 段界限0xFFFFF
    dw  0x8000      ; base address = 0x0B8000
    dw  0x920B      ; code read/write
    dw  0x0040      ; granularity=1024

GdtLen		equ	$ - LABEL_GDT                       ;GDT长度
GdtPtr		dw	GdtLen - 1				            ;GDT界限
		    dd	BaseOfSetupPhyAddr + LABEL_GDT		;GDT基地址

; GDT 选择子
SelectorFlatC		equ	LABEL_DESC_FLAT_C	- LABEL_GDT
SelectorFlatRW		equ	LABEL_DESC_FLAT_RW	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT

start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, BaseOfStack

    ; 准备进入保护模式
    lgdt [GdtPtr]           ; 加载GDTR
    cli                     ; 关中断

    in al, 92h              ; 打开地址线A20
    or al, 00000010b
    out 92h, al

    ; 准备切换到保护模式
    mov eax, cr0
    or  eax, 1
    mov cr0, eax

    ; 真正进入保护模式
    jmp dword SelectorFlatC:(BaseOfSetupPhyAddr+pm_start)
    

[SECTION .s32]

ALIGN 32

[BITS 32]

pm_start:
    mov ax, SelectorVideo
    mov gs, ax

    mov ax, SelectorFlatRW
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov esp, TopOfStack 
    mov ah, 0Fh
    mov al, 'P'
    mov [gs:((80 * 0 + 39) * 2)], ax

    jmp $


[SECTION .data]
    
; 堆栈在数据段末尾
StackSpace:     times 1024 db 0
TopOfStack      equ BaseOfSetupPhyAddr+$ ; 栈顶

times 2046-($-$$) db 0
dw 0xaaaa

