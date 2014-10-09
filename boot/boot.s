org 07c00h

BaseOfSetup        equ         09000h ; setup.bin 被加载到的位置 --- 段地址
OffsetOfSetup      equ         0100h  ; setup.bin 被加载到的位置 --- 偏移地址
SetupLen           equ         4      ; setup.bin 的长度，4个扇区
BaseOfKernelFile    equ         08000h ; kernel.bin 被加载到的位置 --- 段地址
OffsetOfKernelFile  equ         0h     ; kernel.bin 被加载到的位置 --- 偏移地址
KernelLen           equ         20     ; kernel.bin 的长度，20个扇区
BaseOfStack		    equ	        07c00h ; Boot状态下堆栈基地址(栈底, 从这个位置向低地址生长)


jmp start


start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, BaseOfStack

    ; 清屏
    mov ax, 0600h   ; AH = 6, AL = 0h
    mov bx, 0700h   ; 黑底白字(BL = 07h)
    mov cx, 0       ; 左上角:(0,0)
    mov dx, 0184fh  ; 右下角:(80, 50)
    int 10h

    ; 软驱复位
    xor ah, ah
    xor dl, dl
    int 13h

; 加载 Setup.bin, 位于软驱第2到第5个扇区
load_Setup:
    mov ax, BaseOfSetup
    mov es, ax
    mov bx, OffsetOfSetup  ; es:bx = BaseOfsetup:OffsetOfLoader
    mov ah, 02h
    mov al, SetupLen
    mov cx, 0x0002      ; sector 2, track 0 (CH=0, CL=2)
    mov dx, 0x0000      ; drive 0, head 0 (DH=0, DL=0)
    int 13h
    jnc ok_load_Setup
    xor ah, ah          ; reset the diskette
    xor dl, dl
    int 13h
    jmp load_Setup

ok_load_Setup:

    ; print "Booting..."
    mov ax, BootMessage
    mov bp, ax
    mov ax, ds
    mov es, ax          ; ES:BP = 串地址
    mov cx, MessageLength
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov	dl, 0
	int	10h			; int 10h


; 加载 kernel.sys， 位于软驱第6到第25个扇区
load_Kernel:
    mov ax, BaseOfKernelFile
    mov es, ax
    mov bx, OffsetOfKernelFile  ; es:bx = BaseOfKernelFile:OffsetOfKernelFile
    mov si, 0   ; 记录已读取的扇区数

    mov ch, 0   ; 柱面0
    mov dh, 0   ; 磁头0
    mov cl, 6   ; 扇区6
.read:
    mov ah, 0x02
    mov al, 1   ; 读1个扇区
    mov dl, 0x00      ; A 驱动器
    int 13h
    add si, 1
    cmp si, KernelLen  ; 是否读了20个扇区
    je ok_load_Kernel

.next:
    add bx, 0x200   ; 把内存地址向后移动0x200 即512字节
    add cl, 1       ; 扇区加1
    cmp cl, 18
    jbe .read       ; if cl <= 18, jump to .read
    mov cl, 1
    add dh, 1
    cmp dh, 2
    jb .read        ; if dh < 2 , jump to .read
    mov dh, 0
    add ch, 1
    cmp ch, 80
    jb .read        ; if ch < 80, jump to .read


ok_load_Kernel:

     jmp BaseOfSetup:OffsetOfSetup       ; 跳转去执行 setup.bin
    ;jmp $

; 关闭软驱
;kill_motor:
;    push dx
;    mov dx, 03F2h
;    mov al, 0
;    out dx, dl
;    pop dx
;    ret

MessageLength       equ   10
BootMessage         db    "Booting..."  ; 10字节

times 510-($-$$)  db 0
dw 0xaa55
