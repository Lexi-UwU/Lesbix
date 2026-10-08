

#include "../uart/uart.h"

#include "../file/filesystem.h"

#include "../tools/utils.h"

#include "basic_graphics.h"

#define LESBIX_BASIC_MAX_LINES 64

static int current_line = 0;

static char lines[LESBIX_BASIC_MAX_LINES][128];




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


// At the top of basic.c
static int variables[26] = {0};

// Helper to look up or store variables ('A'-'Z' or 'a'-'z')
static int get_var_index(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    return -1;
}

static int evaluate_expression(const char *expr) {
    // Skip leading whitespace
    while (*expr == ' ' || *expr == '\t' || *expr == '\r' || *expr == '\n') expr++;

    if (*expr == '\0') return 0;

    // ----------------------------------------------------
    // Pass 1: Look for lowest precedence operators (+ and -)
    // ----------------------------------------------------
    const char *op = NULL;
    char op_type = '\0';

    const char *p = expr;
    // Skip optional leading sign (-5 or +10)
    if (*p == '+' || *p == '-') p++;

    while (*p != '\0') {
        if (*p == '+' || *p == '-') {
            op = p;
            op_type = *p;
        }
        p++;
    }

    // ----------------------------------------------------
    // Pass 2: If no + or -, look for higher precedence (* and /)
    // ----------------------------------------------------
    if (!op) {
        p = expr;
        while (*p != '\0') {
            if (*p == '*' || *p == '/') {
                op = p;
                op_type = *p;
            }
            p++;
        }
    }

    // ----------------------------------------------------
    // If an operator was found, split and evaluate left and right sides
    // ----------------------------------------------------
    if (op) {
        char left_buf[64];
        int left_len = op - expr;

        if (left_len >= (int)sizeof(left_buf)) left_len = sizeof(left_buf) - 1;
        strncpy(left_buf, expr, left_len);
        left_buf[left_len] = '\0';

        int left_val  = evaluate_expression(left_buf);
        int right_val = evaluate_expression(op + 1);

        switch (op_type) {
            case '+': return left_val + right_val;
            case '-': return left_val - right_val;
            case '*': return left_val * right_val;
            case '/': return (right_val != 0) ? (left_val / right_val) : 0;
        }
    }

    // ----------------------------------------------------
    // Base Case: No operators remaining -> Variable or Number
    // ----------------------------------------------------
    int var_idx = get_var_index(*expr);
    if (var_idx != -1) {
        // Ensure it's a standalone variable name, not part of a word
        const char *next = expr + 1;
        while (*next == ' ' || *next == '\t' || *next == '\r' || *next == '\n') next++;
        if (*next == '\0') {
            return variables[var_idx];
        }
    }

    // Numeric constant
    return atoi(expr);
}

//RETURN CODE:

// 0: Failure
// 1: Success (Increment Counter)
// 2: Success (Dont Increment Counter)
// 3: Halt
static int LESBIX_BASIC_RUN_LINE(const char *line) {

    const char *p;
    char buf[2];
    int target_line;
    //print_uart0(line);
    //print_uart0("\n");

    if (strcmp(line, "END") == 0) {
        return 3;
    }

    else if (strcmp(line, "ENDPROC") == 0) {
        return 3;
    }

    else if (strncmp(line, "DEF ", 4) == 0) {
        return 1;
    }


    // CLS (Clear Screen stub)
    else if (strcmp(line, "CLS") == 0) {
        // Call your screen clear UART / display driver routine here
        return 1;
    }

    else if (strncmp(line, "MODE ", 5) == 0) {
        return 1;
    }

    else if (strncmp(line, "GCOL ", 5) == 0) {
        return 1;
    }

    else if (strncmp(line, "MOVE ", 5) == 0) {
        return 1;
    }

    //else if (strncmp(line, "PLOT ", 4) == 0) {
    //    return 1;
    //}

    else if (strncmp(line, "PROC ", 4) == 0) {
        return 1;
    }

    else if (strncmp(line, "IF ", 3) == 0) {
        return 1;
    }
    else if (strncmp(line, "PLOT ", 5) == 0) {
        const char *p = line + 5;
        int args[5] = {0};
        int arg_count = 0;

        while (*p != '\0' && arg_count < 5) {
            // Skip leading spaces and commas
            while (*p == ' ' || *p == '\t' || *p == ',') p++;
            if (*p == '\0') break;

            // Extract the argument token
            char token[32];
            int idx = 0;
            while (*p != '\0' && *p != ',' && *p != ' ' && *p != '\t' && idx < 31) {
                token[idx++] = *p++;
            }
            token[idx] = '\0';

            args[arg_count++] = evaluate_expression(token);
        }

        if (arg_count == 5) {
            LESBIX_BASIC_GRAPHICS_SET_PIXEL(args[0], args[1], args[2], args[3], args[4]);
            return 1;
        }

        print_uart0("Syntax Error: PLOT requires 5 arguments (X, Y, R, G, B)\n");
        return 0;
    }
    else if (strchr(line, '=') != NULL) {
        const char *p = line;

        // Optional: Skip optional "LET " prefix if present
        if (strncmp(p, "LET ", 4) == 0) {
            p += 4;
        }

        // Skip spaces
        while (*p == ' ' || *p == '\t') p++;

        // Get variable name
        int var_idx = get_var_index(*p);
        if (var_idx == -1) {
            print_uart0("Syntax Error: Invalid variable name\n");
            return 0;
        }

        // Find the '=' sign
        const char *eq = strchr(p, '=');
        if (!eq) return 0;

        // Evaluate expression after '='
        int value = evaluate_expression(eq + 1);

        // Store value in state table
        variables[var_idx] = value;
        return 1;
    }



    // Check if command starts with "PRINT "
    else if (strncmp(line, "PRINT ", 6) == 0) {
        const char *arg = line + 6;
        while (*arg == ' ' || *arg == '\t') arg++;

        // Case A: Quoted String -> PRINT "HELLO"
        if (*arg == '"') {
            const char *start = arg + 1;
            const char *end = strrchr(start, '"');
            if (end) {
                for (p = start; p < end; p++) {
                    buf[0] = *p;
                    buf[1] = '\0';
                    print_uart0(buf);
                }
            }
        }
        // Case B: Variable or Expression -> PRINT A
        else {
            int val = evaluate_expression(arg);
            char str_buf[20];
            int_to_str(val, str_buf);
            print_uart0(str_buf);
        }

        print_uart0("\n");
        return 1;
    }

    else if (strncmp(line, "GOTO ", 5) == 0) {
        const char *num_str = line + 5;

        // Skip any leading whitespace after "GOTO "
        while (*num_str == ' ') {
            num_str++;
        }

        // Convert line number string to an integer
        target_line = atoi(num_str);

        if (target_line > 0) {
            // TODO: Execute your jump logic here (e.g., jump_to_line(target_line);)
            current_line = target_line;
        } else {
            print_uart0("Syntax Error: Invalid line number for GOTO\n");
        }

        return 2;
    }

    else {
        print_uart0("INVALID COMMAND: ");
        print_uart0(line);
        print_uart0("\n");

    }

    return 1;
}


void LESBIX_BASIC_RUN(){

    int return_code;


    LESBIX_BASIC_GRAPHICS_SET_TILE(4,4,'2');


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

    if (line_number < 0 || line_number >= LESBIX_BASIC_MAX_LINES) {
        print_uart0("Syntax Error: Line number out of range\n");
        return;
    }

    // Skip whitespace after the number
    while (*ptr == ' ' || *ptr == '\t') {
        ptr++;
    }


    if ((ptr[0] == 'R' || ptr[0] == 'r') &&
        (ptr[1] == 'E' || ptr[1] == 'e') &&
        (ptr[2] == 'M' || ptr[2] == 'm') &&
        (ptr[3] == ' ' || ptr[3] == '\t' || ptr[3] == '\r' || ptr[3] == '\n' || ptr[3] == '\0')) {
        return;
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
    DriverResponse file_resp;
    char *line;
    char *file_contents;

    FILESYSTEM_GET_FILE(path, &file_resp);

    file_contents = file_resp.data_char;

    if (!file_contents) return;

    // Tokenize by carriage return and newline
    line = strtok(file_contents, "\r\n");

    while (line != NULL) {
        // Process the null-terminated line string directly


        LESBIX_LOAD_LINE(line);

        line = strtok(NULL, "\r\n");
    }
}



