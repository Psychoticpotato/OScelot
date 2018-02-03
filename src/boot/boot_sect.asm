[org 0x7c00]  ;this boot sector will load 32-bit protected mode
[bits 16]

KERNEL_OFFSET equ 0x1000    ;Memory offset to which we load the kernel

mov [BOOT_DRIVE], dl    ;BIOS stores boot drive in DL; useful info to remember

mov bp, 0x9000    ;Set the stack
mov sp, bp

  mov bx, MSG_REAL_MODE
  call print_string
  call print_nl

  call load_kernel

  call switch_to_pm     ;The Point of No Return

  jmp $
    ;16-bit includes
    %include "boot/print_string.asm"
    %include "boot/disk_load.asm"
    ;Now, 32-bit includes
    %include "boot/GDT.asm"
    %include "boot/switch_to_pm.asm"
    %include "boot/Print_32.asm"


[bits 16]

load_kernel:

  mov bx, MSG_LOAD_KERNEL
  call print_string
  call print_nl

  mov bx, KERNEL_OFFSET     ;Set up parameters for our disk_load routine
  mov dh, 15                ; We load the first 15 sectors (excluding boot sector)
  mov dl, [BOOT_DRIVE]      ;To address KERNEL_OFFSET
  call disk_load

  ret

[bits 32]
  ;We should now be in 32-bit
  BEGIN_PM:

  mov ebx, MSG_PROT_MODE
  call print_string_pm  ;Woohoo, 32 bit printer

  call KERNEL_OFFSET    ;Now, jump to address of our loaded Kernel code
  jmp $

  ; Global Variables
  BOOT_DRIVE db 0
  MSG_PROT_MODE: db "Successfully loaded 32-bit Protected mode", 0
  MSG_REAL_MODE: db "Booted in 16-bit Real Mode", 0
  MSG_LOAD_KERNEL: db "Loading Kernel into memory", 0

;Always end with padding and the Magic BIOS number
times 510-($-$$) db 0

dw 0xaa55
