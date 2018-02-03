[bits 32]       ;Always explicitly state what bits we are in
global _start   ;Declare that we are referencing the external
                ;symbol 'start', so the linker will substitute the final address

_start:
  [extern kernel_main]
  call kernel_main
  jmp $
