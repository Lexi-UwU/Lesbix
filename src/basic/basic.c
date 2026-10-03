

#include "../uart/uart.h"

#include "../file/filesystem.h"

#include "../tools/utils.h"

#define LESBIX_BASIC_MAX_LINES 128

static int current_line = 0;

static char lines[LESBIX_BASIC_MAX_LINES][128];




//RETURN CODE:

// 0: Failure
// 1: Success (Increment Counter)
// 2: Success (Dont Increment Counter)
// 3: Halt
static int LESBIX_BASIC_RUN_LINE(const char *line) {
    //print_uart0(line);
    //print_uart0("\n");

    if (strcmp(line, "END") == 0) {
        return 3;
    }

    // Check if command starts with "PRINT "
    if (strncmp(line, "PRINT ", 6) == 0) {
        //print_uart0("print");
        const char *start = strchr(line, '"');       // Find first quotation mark
        const char *end = strrchr(line, '"');        // Find last quotation mark


        // Print characters between the quotes
        for (const char *p = start + 1; p < end; p++) {
            char buf[2] = { *p, '\0' };
            print_uart0(buf);
        }
        print_uart0("\n");

        return 1;
    }

    if (strncmp(line, "GOTO ", 5) == 0) {
        const char *num_str = line + 5;

        // Skip any leading whitespace after "GOTO "
        while (*num_str == ' ') {
            num_str++;
        }

        // Convert line number string to an integer
        int target_line = atoi(num_str);

        if (target_line > 0) {
            // TODO: Execute your jump logic here (e.g., jump_to_line(target_line);)
            current_line = target_line;
        } else {
            print_uart0("Syntax Error: Invalid line number for GOTO\n");
        }

        return 2;
    }

    return 1;
}


void LESBIX_BASIC_RUN(){

    int return_code;





    current_line = 0;


    while (current_line < LESBIX_BASIC_MAX_LINES) {

        if (lines[current_line][0] == '\0') {
            current_line++;
            continue;
        }



        return_code = LESBIX_BASIC_RUN_LINE(lines[current_line]);

        if (return_code == 0){

            //Failure (Maybe use success flags?)
        }
        else if (return_code == 1){
            // Success (Increment Counter)
            current_line = current_line + 1;
        }
        else if (return_code == 2){
            //Success (Dont Increment Counter)
        }
        else if (return_code == 3){
            //Halt
            return;
        }

    }
    return;

}



// Simple helper to convert an integer to a string buffer

//TODO: MOVE THIS FUNCTION TO A DIFFERENT FILE

// Method: Pass a destination buffer from the caller (Stack safe, no dynamic memory)
static void int_to_str(long num, char *out_str) {
    char temp[20];
    int i = 0, j = 0;

    if (num == 0) {
        out_str[0] = '0';
        out_str[1] = '\0';
        return;
    }

    while (num > 0) {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Reverse digits into target string
    while (i > 0) {
        out_str[j++] = temp[--i];
    }
    out_str[j] = '\0';
}

static void LESBIX_LOAD_LINE(const char *line){
    const char *ptr = line;
    long line_number = 0;
    int found_digits = 0;

    // Skip leading spaces
    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\r') {
        ptr++;
    }

    // Parse digits into integer
    while (*ptr >= '0' && *ptr <= '9') {
        line_number = (line_number * 10) + (*ptr - '0');
        found_digits = 1;
        ptr++;
    }

    if (!found_digits) {
        print_uart0("Syntax Error: Missing line number\n");
        return;
    }

    // Skip whitespace after the number
    while (*ptr == ' ' || *ptr == '\t') {
        ptr++;
    }

    // Allocate buffer on stack and convert
    //char num_buf[20];
    //int_to_str(line_number, num_buf);

    //print_uart0("Line number: ");
    //print_uart0(num_buf);
    //print_uart0("\nInstruction: ");
    //print_uart0(ptr);
    //print_uart0("\n");


    strncpy(lines[line_number], ptr, sizeof(lines[line_number]) - 1);
    lines[line_number][sizeof(lines[line_number]) - 1] = '\0';
}



void LESBIX_BASIC_LOAD_PROGRAM(const char *path){
    char *file_contents = FILESYSTEM_GET_FILE(path)->data_char;

    if (!file_contents) return;

    // Tokenize by carriage return and newline
    char *line = strtok(file_contents, "\r\n");

    while (line != NULL) {
        // Process the null-terminated line string directly


        LESBIX_LOAD_LINE(line);

        line = strtok(NULL, "\r\n");
    }
}



