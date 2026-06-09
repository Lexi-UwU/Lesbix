.section .text
.global _Reset
_Reset:
    LDR sp, =stack_top    @ Load address of top of stack
    BL main               @ Call main
    B .                   @ Infinite loop

.section .bss
.align 8                  @ Force 8-byte alignment
stack_bottom:
    .skip 1024            @ Reserve 1KB
stack_top:
