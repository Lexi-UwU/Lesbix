//Hazelut Programming Language Virtual Machine

//The system uses a 32 bit address space

//Format:

//ADDRESS  : INSTRUCTION : ADDR 1   : ADDR 2

//FFFFFFFF : FFFF    	 : FFFFFFFF : FFFFFFFF


#ifndef FILESYSTEM_MANAGER
    #include "../file/filesystem.h"
#endif


#ifndef UART_C
    #include "../uart/uart.h"
#endif

#ifndef HVM_CORE
    #include "../hazelnut/hazelnut_vm.h"
#endif


#include "../tools/utils.h"






//More functionality needs to be added in the future
//Run Command
void command_hazelnut(const char *s) {
    if (s == NULL || s[0] == '\0') {
        print_uart0("Error: No file specified.\n");
        return;
    }

    char *full_path = FILESYSTEM_MERGE_PATHS(FILESYSTEM_CURRENT_WORKING_DIRECTORY, s);
    if (full_path == NULL) {
        print_uart0("Error: Path resolution failed.\n");
        return;
    }

    DriverResponse file = FILESYSTEM_GET_FILE(full_path);

    // FIX: Check for failure BEFORE printing and BEFORE calling the VM
    if (file.data_int == NULL) {
        if (file.data_char) {
            print_uart0(file.data_char); // Print the driver's error message
        } else {
            print_uart0("Error: File not found or failed to load.\n\r");
        }
        return; // STOP HERE. Do not call hazlenut_run_file.
    }

    // Now it's safe to run
    hazlenut_run_file(file.data_int);
}

