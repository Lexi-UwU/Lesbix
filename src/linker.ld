ENTRY(_Reset)
SECTIONS
{
    . = 0x10000;
    .text : {
        CMakeFiles/kernel.elf.dir/src/startup.s.obj(.text)
        *(.text)
    }
    .data : { *(.data) }
    .bss  : { *(.bss) }

    _end = .;
}
