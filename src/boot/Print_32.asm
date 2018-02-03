[bits 32]
;Define constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

;Print a null terminated string pointed to by EDX
print_string_pm:
  pusha
  mov edx, VIDEO_MEMORY   ;Ensure that we start at the start of video memory

print_string_pm_loop:
  mov al, [ebx]           ;Store the char at EBX in AL
  mov ah, WHITE_ON_BLACK  ;Store the attribute in AH

  cmp al, 0               ;if (al == 0), we are done.
  je print_string_pm_done

  mov [edx], ax           ; Store char and attriubutes at current char cell
  add ebx, 1              ; Increment EBX to the next character
  add edx, 2              ; Move to next character cell in video memory

jmp print_string_pm_loop  ;Loop and prepare for the next

print_string_pm_done:
popa
ret