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
#include "hazelnut/hazelnut_out.h"


//hazelnut output.hazelnut

int command_hazelnut_init() {
    return hazelnut_vm_init();
}


//More functionality needs to be added in the future
//Run Command
void command_hazelnut(const char *s) {

    char buffer[64];
    // Copy input safely
    strncpy(buffer, s, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // 1. Tokenize to find the command
    char *cmd = strtok(buffer, " ");

    // 2. Check if the command is "echo"
    if (cmd != NULL && strcmp(cmd, "hazelnut") == 0) {
        // 3. Get the rest of the string as the argument
        char *arg = strtok(NULL, "");

        if (s == NULL || s[0] == '\0') {
            print_uart0("Error: No file specified.\n\r");
            return;
        }

        char *full_path = FILESYSTEM_MERGE_PATHS(FILESYSTEM_CURRENT_WORKING_DIRECTORY, arg);
        if (full_path == NULL) {
            print_uart0("Error: Path resolution failed.\n");
            return;
        }

        DriverResponse file = FILESYSTEM_GET_FILE(full_path);

        if (file.response_code == 0) {
            hvm_print("Response code is NULL\n\r");
            return;
        }
        hvm_print_int(&file.response_code);
        hvm_print("\n\r");

        if (file.response_code == 10) {
            hvm_print(file.data_char);
            hvm_print("\n\r");

        }


        // FIX: Check for failure BEFORE printing and BEFORE calling the VM
        if (file.data_char == NULL) {
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
}

