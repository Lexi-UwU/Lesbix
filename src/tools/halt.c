
#define TOOLS_HALT
void halt(void) {
    while (1) {
        // Optional: Call architecture-specific instructions
        //__asm__("wfi"); // ARM 'Wait For Interrupt'
        __asm__ volatile ("mcr p15, 0, r0, c7, c0, 4");
        //__asm__("hlt");  // x86 'Halt'
    }
}
