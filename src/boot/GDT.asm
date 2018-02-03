; GDT
gdt_start:

gdt_null: ;Mandatory null descriptor
  dd 0x0    ; 'dd' means define double word (4 bytes)
  dd 0x0

gdt_code: ; Code segment descriptor
  ; base=0x0, limit=0xfffff,
  ; 1st flags: (present)1 (priviledge) 00 (descriptor type)1 ->1001b
  ;Type flags: (code)1 (conforming)0 (readable)1 (accessed)0 ->1010b
  ; 2nd flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 ->1100b
  dw 0xffff    ; limit (bits 0-15)
  dw 0x0       ; Base (bits 0-15)
  db 0x0       ; Base (bits 16-23);
  db 10011010b ; 1st flags, type flags
  db 11001111b ; 2nd flags, limit (bits 16-19)
  db 0x0       ; Base (bits 24-31)

gdt_data:    ; the data segment descriptor
  ; Same as code segment except for the type flags:
  ; Type Flags: (code)0 (expand_down)0 (writable)1 (accessed)0 -> 0010b
  dw 0xffff    ; limit (bits 0-15)
  dw 0x0       ; Base (bits 0-15)
  db 0x0       ; Base (bits 16-23);
  db 10010010b ; 1st flags, type flags
  db 11001111b ; 2nd flags, limit (bits 16-19)
  db 0x0       ; Base (bits 24-31)

gdt_end:     ; The assembler can now calculate the size of the GDT for the descriptor.

; GDT descriptor
gdt_descriptor:
  dw gdt_end - gdt_start - 1   ;Size of the GDT, always minus one of the true Size
  dd gdt_start                 ;Start address of the GDT

[GLOBAL idt_flush]    ;Allows the C to call idt_flush()

idt_flush:
  mov eax, [esp+4]    ;Get pointer to the IDT as a parameter.
  lidt [eax]          ;Load the IDT pointer
  ret

;Define handy constants for the GDT segment descriptor offsets.
;THese are what segment registers must contain when in protected mode.
;Example: when we set DS = 0x10 in PM, the cpu knows that we mean to use
;the segment described at offset 0x10 (16 bytes) in our GDT, which in
;our case is the DATA segment (0x0 0> NULL; 0x08 -> CODE; 0x10 -> DATA
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
