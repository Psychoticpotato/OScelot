; Load DH sectors to ES;BX from drive DL
disk_load:
  push dx       ;store DX to stack so we can recall
                ;how many sectors were requested to be read,
                ;even if it is altered in the meantime
  mov ah, 0x02  ;BIOS read sector function
  mov al, dh    ;Read DH sectors
  mov ch, 0x00  ;Select cylinder 0
  mov dh, 0x00  ;Select head 0
  mov cl, 0x02  ;Start reading from second sector (after the boot sector)

  int 0x13      ;BIOS interrupt

  jc disk_error ;Jump if disk_error

  pop dx        ;Restore DX from stack
  cmp dh, al    ;If AL (sectors read) != DH (Sectors expected), then
  jne disk_error;display errorfuckyouautocomplete
  ret

  disk_error:

    mov bx, DISK_ERROR_MSG
    call print_string
    call print_nl

    disk_loop:
    jmp $

    ;Variables
    DISK_ERROR_MSG db "Disk read error", 0