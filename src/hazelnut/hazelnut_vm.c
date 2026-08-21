#define HVM_CORE

#ifndef HVM_ABSTRACTION
    #include "hazelnut_out.h"
#endif

#include <stdio.h>

#include "../tools/utils.h"

#include "hazelnut_objects.h"



#include <stdlib.h>

int hazelnut_script_size = 5;
int hazelnut_script_count = 0;
struct hazelnut_script *hazelnut_script_array = NULL;


int hazelnut_vm_init(void) {
    hazelnut_script_array = malloc(hazelnut_script_size * sizeof(int));
    if (!hazelnut_script_array) return -1;
    return 0;
}


void add_hazelnut_script(struct hazelnut_script script) {
    if (hazelnut_script_count >= hazelnut_script_size) {
        int new_size = hazelnut_script_size * 2;
        struct hazelnut_script *temp = realloc(
            hazelnut_script_array,
            new_size * sizeof(struct hazelnut_script)
        );

        // Return without corrupting hazelnut_script_size if memory fails
        if (!temp) return;

        hazelnut_script_array = temp;
        hazelnut_script_size = new_size;
    }

    // Append the new script element to the array
    hazelnut_script_array[hazelnut_script_count] = script;
    hazelnut_script_count++;
}

void hazelnut_proccess_byte(unsigned int byte, struct hazelnut_script *script) {
    unsigned int count = script->internal_counter.byte_count;

    // 1. Signature check (Bytes 0 - 7)
    if (count == 7) {
        script->internal_counter.passed_identifier = 1;
    }

    // Bytes 16-23 contain the literal string key "length\0\0".
    // We clear key_count before byte 24 so the key string isn't shifted into the number.
    if (count == 23) {
        script->header.key_count = 0;
    }

    // 2. Accumulate actual key count value (Bytes 24 through 31)
    if (count >= 24 && count <= 31) {
        script->header.key_count = (script->header.key_count << 8) | (byte & 0xFF);
    }

    // 3. Mark length header parsed AFTER byte 31 is fully processed
    if (count == 31) {
        script->internal_counter.passed_headersize = 1;
    }

    // --- STATE MACHINE ---
    if (count >= 32) {
        /* Bytes 32+: Header Entries & Instructions */

        // Header Allocation happens strictly on Byte 32
        if (script->header.objects == NULL && script->header.key_count > 0) {
            script->header.objects = calloc(script->header.key_count, sizeof(struct hazelnut_script_header_object));

            if (script->header.objects == NULL) {
                return;
            }

            script->internal_counter.header_index = 0;
            script->internal_counter.header_progress = 0;
        }

        unsigned int idx = script->internal_counter.header_index;
        unsigned int progress = script->internal_counter.header_progress;

        // Parse 16-byte Header Objects (8 bytes key + 8 bytes value)
        if (script->header.objects != NULL && idx < script->header.key_count) {
            if (progress < 8) {
                script->header.objects[idx].key[progress] = (char)byte;
            }
            else if (progress < 16) {
                script->header.objects[idx].value[progress - 8] = (unsigned char)byte;
            }

            script->internal_counter.header_progress++;

            if (script->internal_counter.header_progress >= 16) {
                script->internal_counter.header_progress = 0;
                script->internal_counter.header_index++;
            }
        }
        else {
            /* Instructions / Code Section */

            // 1. First-time instruction block allocation
            if (script->objects == NULL) {
                int inst_count = 0;

                // Match against "unixtime" key written by Python createHeader()
                for (int i = 0; i < script->header.key_count; i++) {
                    if (memcmp(script->header.objects[i].key, "unixtime", 8) == 0) {
                        unsigned char *val = script->header.objects[i].value;
                        // Bytes 4..7 in value array contain the total instruction count
                        inst_count = (val[4] << 24) | (val[5] << 16) | (val[6] << 8) | val[7];
                        break;
                    }
                }

                // Default fallback if count isn't explicitly resolved
                if (inst_count <= 0) {
                    inst_count = 512;
                }

                script->objects = calloc(inst_count, sizeof(struct hazelnut_script_object));
                if (script->objects == NULL) return;

                script->internal_counter.instruction_index = 0;
                script->internal_counter.instruction_progress = 0;
            }

            // 2. Decode the 16-byte fixed instruction chunks into the objects array
            int i_idx = script->internal_counter.instruction_index;
            int i_prog = script->internal_counter.instruction_progress;

            struct hazelnut_script_object *obj = &script->objects[i_idx];

            // Reset field values on progress start to clear old data
            if (i_prog == 0)  obj->address = 0;
            if (i_prog == 4)  obj->opcode = 0;
            if (i_prog == 6)  obj->operand1 = 0;
            if (i_prog == 10) obj->operand2 = 0;
            if (i_prog == 14) obj->debug = 0;

            if (i_prog < 4) {
                // Address Location (4 bytes)
                obj->address = (obj->address << 8) | (byte & 0xFF);
            }
            else if (i_prog < 6) {
                // Opcode (2 bytes packed big-endian into int, e.g., 'nw', 'st', 'pr')
                obj->opcode = (obj->opcode << 8) | (byte & 0xFF);
            }
            else if (i_prog < 10) {
                // Operand 1 (4 bytes)
                obj->operand1 = (obj->operand1 << 8) | (byte & 0xFF);
            }
            else if (i_prog < 14) {
                // Operand 2 (4 bytes)
                obj->operand2 = (obj->operand2 << 8) | (byte & 0xFF);
            }
            else if (i_prog < 16) {
                // Debug metadata (2 bytes)
                obj->debug = (obj->debug << 8) | (byte & 0xFF);
            }

            script->internal_counter.instruction_progress++;

            // Advance to next instruction frame every 16 bytes
            if (script->internal_counter.instruction_progress >= 16) {
                script->internal_counter.instruction_progress = 0;
                script->internal_counter.instruction_index++;
            }
        }
    }

    script->internal_counter.byte_count++;
}


struct hazelnut_script_object *get_instruction_by_address(struct hazelnut_script *script, int target_address) {
    if (script == NULL || script->objects == NULL) return NULL;

    // Search through instruction_index (or instruction count)
    int total_instructions = script->internal_counter.instruction_index;

    for (int i = 0; i < total_instructions; i++) {
        if (script->objects[i].address == target_address) {
            return &script->objects[i]; // Return pointer to matching object
        }
    }

    return NULL; // Address not found
}

int hazelnut_tick_script(struct hazelnut_script *script) {
    if (!script) return 1;

    struct hazelnut_script_object *instruction_object = get_instruction_by_address(script, script->program_counter);

    if (instruction_object == NULL) {
        //hvm_print("Error: No instruction found at address ");
        //hvm_print_int(&script->program_counter);
        //hvm_print("\n\r");

        // Advance PC or halt execution to break infinite loop
        script->program_counter++;
        return -1;
    }

    char opcode_str[3] = {
        (char)(instruction_object->opcode >> 8),
        (char)(instruction_object->opcode & 0xFF),
        '\0'
    };
    hvm_print("\n\rCURRENT OPCODE: ");
    hvm_print(opcode_str);

    hvm_print("\n\rCURRENT PARAM1: \n\r");
    hvm_print_int(&instruction_object->operand1);
    hvm_print("\n\r");

    hvm_print("\n\rCURRENT PARAM2: \n\r");
    hvm_print_int(&instruction_object->operand2);
    hvm_print("\n\r");

    script->program_counter++;

    return 0;
}

void hazlenut_run_file(int *s){

    if (s == NULL) {
        hvm_print("Error: Invalid or uninitialized file pointer.\n");
        return;
    }

    // Cast the start pointer to inspect memory as raw 8-bit bytes
    unsigned char *byte_ptr = (unsigned char *)s;

    // Loop until we hit the -1 terminator in 32-bit integer form

    struct hazelnut_script hazelnut_script = {0};


    while (*(int *)byte_ptr != -1) {
        unsigned char current_byte = *byte_ptr;

        // Process individual byte (e.g., format and print)
        char buf[8];
        snprintf(buf, sizeof(buf), "0x%02X ", current_byte);
        hvm_print(buf);
        hazelnut_proccess_byte(current_byte,&hazelnut_script);

        byte_ptr++; // Advance to the next 8-bit byte
    }

    while (1) {
        int return_value = hazelnut_tick_script(&hazelnut_script);
        if (return_value > 0) {
            return;
        }
    }
    hvm_print("\n\r");
}
