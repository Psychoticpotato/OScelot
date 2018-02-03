all:
#TODO: Make Clean

#Flags for the gcc compiler
gccFlags 		:= -fno-pie -m32 -ffreestanding

#Set up the src and bin directories
src_root 		:= src
bin_root 		:= bin

#This is left blank so that new folders can be added as required
subDirs			:= $(sort $(dir $(wildcard $(src_root)/*/)))

#TODO: Allow for multiple subfolders

#Replace the src root with nothing
subDirs			:= $(subst $(src_root)/,,$(subDirs))
subDirs			:= $(subst /,,$(subDirs))
$(info subDirs=$(subDirs))

#Begin setting up suffixes for files
cSrcSuf 	:= .c
cHeadSuf 	:= .h
asmSrcSuf	:= .asm
sSrcSuf		:= .s

cOutSuf		:= .o
asmOutSuf	:= .bin
sOutSuf		:= .o

#Build all source file paths
cSource		:= $(foreach d, $(addprefix $(src_root)/,$(subDirs)), $(wildcard $(addprefix $d/*, $(cSrcSuf))))
asmSource	:= $(foreach d, $(addprefix $(src_root)/,$(subDirs)), $(wildcard $(addprefix $d/*, $(asmSrcSuf))))
sSource		:= $(foreach d, $(addprefix $(src_root)/,$(subDirs)), $(wildcard $(addprefix $d/*, $(sSrcSuf))))

#Output this info to the console for debugging
$(info cSource=$(cSource))
$(info asmSource=$(cSource))
$(info sSource=$(sSource))

#Move on to Source -> object file mapping
#Map $(src_root) -> $(bin_root) with identical directories
cObj		:= $(addsuffix $(cOutSuf),   $(basename $(patsubst $(src_root)%, $(bin_root)%, $(cSource))))
asmBin		:= $(addsuffix $(asmOutSuf), $(basename $(patsubst $(src_root)%, $(bin_root)%, $(asmSource))))
sObj		:= $(addsuffix $(sOutSuf),   $(basename $(patsubst $(src_root)%, $(bin_root)%, $(sSource))))


$(info cObj=$(cObj))
$(info asmBin=$(asmBin))
$(info sObj=$(sObj))

#Generate rules for every .o and .bin file
$(foreach s, $(cSource),   $(foreach o, $(filter %$(basename $(notdir $s)).o,$(cObj)),     $(info New Rule: $o: $s)$(eval $o: $s)))
$(foreach s, $(asmSource), $(foreach o, $(filter %$(basename $(notdir $s)).bin,$(asmBin)), $(info New Rule: $o: $s)$(eval $o: $s)))
$(foreach s, $(sSource),   $(foreach o, $(filter %$(basename $(notdir $s)).o,$(sObj)),     $(info New Rule: $o: $s)$(eval $o: $s)))

$(cObj):   ; $(if $(wildcard $(@D)),,mkdir -p $(@D) &&) gcc $(gccFlags) -c $< -o $@
$(asmBin): ; $(if $(wildcard $(@D)),,mkdir -p $(@D) &&) nasm $< -i $(src_root)/ -f bin -o $@
$(sObj):   ; $(if $(wildcard $(@D)),,mkdir -p $(@D) &&) nasm $< -i $(src_root)/ -f elf -o $@
	
$(info cObj=$(cObj))


all: os-image

run: os-image
	qemu-system-i386 -fda os-image

#Begin making specific rules
os-image: $(bin_root)/boot/boot_sect.bin $(bin_root)/kernel/kernel.bin
	cat $^ > os-image

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
$(bin_root)/kernel/kernel.bin: $(bin_root)/boot/kernel_entry.o $(cObj) $(bin_root)/cpu/interrupt.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
