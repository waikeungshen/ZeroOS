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
    
    call InitKernel

    ;jmp $
    jmp dword SelectorFlatC:KernelEntryPointPhyAddr

; 重新放置内核
InitKernel:
    xor esi, esi
    mov cx, word[BaseOfKernelFilePhyAddr+2ch]
    movzx ecx, cx                                   ; ecx <= pELFHdr->e_phnum, The number of Program header table
    mov esi, [BaseOfKernelFilePhyAddr+1ch]          ; esi <= pELFHdr->e_phoff, Program header table 在文件中的偏移量
    add esi, BaseOfKernelFilePhyAddr                ; Program header table 在内存中的位置
.Begin: 
    mov eax, [esi + 0]
    cmp eax, 0
    jz .NoAction
    push dword [esi + 010h]              ; size
    mov eax, [esi + 04h]
    add eax, BaseOfKernelFilePhyAddr
    push eax                             ; src
    push dword [esi + 08h]               ; dst
    call MemCpy
    add esp, 12     ; 栈指针向下移动12字节

.NoAction:
    add esi, 020h
    dec ecx         ; ecx--
    jnz .Begin

    ret

; ------------------------------------------------------------------------
; 内存拷贝，仿 memcpy
; ------------------------------------------------------------------------
; void* MemCpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
MemCpy:
	push	ebp             ; 保存栈基址指针
	mov	ebp, esp            ; 改变栈地址指针

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	esi, [ebp + 12]	; Source
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		; 判断计数器
	jz	.2		; 计数器为零时跳出

	mov	al, [ds:esi]
	inc	esi         ; esi++
					; 逐字节移动
	mov	byte [es:edi], al
	inc	edi	        ; edi++

	dec	ecx		; 计数器减一
	jmp	.1		; 循环
.2:
	mov	eax, [ebp + 8]	; 返回值

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp        ; 恢复堆栈地址
	pop	ebp             ; 恢复栈基址指针

	ret			; 函数结束，返回

[SECTION .data]
    
; 堆栈在数据段末尾
StackSpace:     times 1024 db 0
TopOfStack      equ BaseOfSetupPhyAddr+$ ; 栈顶

times 2046-($-$$) db 0
dw 0xaaaa

