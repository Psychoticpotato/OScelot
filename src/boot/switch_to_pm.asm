[bits 16]
switch_to_pm:
cli                   ;Clear interrupts
lgdt [gdt_descriptor] ;Let it know where the GDT is located

mov eax, cr0          ;To make the switch to protected mode,
or eax, 0x1           ;We set the first bit of cr0, a control register
mov cr0, eax          ;Now, we update the control register

jmp CODE_SEG:start_protected_mode   ;We must perform a "long" jump so the processor
                                    ;will finish all 16-bit commands


[bits 32]

start_protected_mode:
                      ;Now, we are finally in 32-bit proteced mode
mov ax, DATA_SEG      ;In protected mode, our old segments are meaningless
mov ds, ax            ;We need to point our segment registers to the
mov ss, ax            ;data selector we defined in our GDT
mov es, ax
mov fs, ax
mov gs, ax

mov ebp, 0x90000      ;Update the stack position so it is at the top of free space
mov esp, ebp

call BEGIN_PM
