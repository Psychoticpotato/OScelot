OScelot Readme
==============

Just a basic OS that I have been writing; it doesn't really do much at the moment.
It is mainly my exploration into basic OS development.

Special thanks to [This GitHub Project](https://github.com/cfenollosa/os-tutorial) for their tutorial (and links to other tutorials) on OS Development.


A few notes on this file structure:
- src:  Source Files
- bin:  Binary(output) files.

For Assembly files that are to be compiled straight into ".bin" (binary) output,
give them the suffix of ".asm".  The Makefile will compile them into a ".bin" file.
If you want an ELF file, the suffix will be ".s", and a ".o" file will be created, 
for use in linking to a C file.  Examine the Makefile if you would like more info.


Thanks to the following:
- GNU/Debian  (OS on which I developed)
- GCC  (C compiler)
- nasm (ASM compiler)
- Qemu (Emulator for running the OS)
- Atom (Text editor; lightweight IDE)
- Various Atom packages (if someone wants details, I can add them)
