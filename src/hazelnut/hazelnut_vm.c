#define HVM_CORE

#ifndef HVM_ABSTRACTION
    #include "hazelnut_out.h"
#endif

#ifndef __CC65__
    #include <stdio.h>
#endif


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


void add_hazelnut_script(const struct hazelnut_script *script) {

    int new_size;
    struct hazelnut_script *temp;



    if (hazelnut_script_count >= hazelnut_script_size) {
        new_size = hazelnut_script_size * 2;
        temp = realloc(
            hazelnut_script_array,
            new_size * sizeof(struct hazelnut_script)
        );

        // Return without corrupting hazelnut_script_size if memory fails
        if (!temp) return;

        hazelnut_script_array = temp;
        hazelnut_script_size = new_size;
    }

    // Append the new script element to the array
    memcpy(&hazelnut_script_array[hazelnut_script_count], script, sizeof(struct hazelnut_script));
    hazelnut_script_count++;
}

void hazelnut_set_memory(struct hazelnut_script *script,int address, int value) {
    if (address >= script->memory.memory_size) {
        int new_size = hazelnut_script_size * 2;
        int *temp = realloc(
            hazelnut_script_array,
            new_size * sizeof(int)
        );

        // Return without corrupting hazelnut_script_size if memory fails
        if (!temp) return;


        script->memory.memory_size = new_size;
        script->memory.memory = temp;
    }

    script->memory.memory[address] = value;


}


void hazelnut_proccess_byte(unsigned int byte, struct hazelnut_script *script) {

    unsigned int idx;
    unsigned int progress;
    unsigned char *val;
    unsigned int count = script->internal_counter.byte_count;
    int i;
    int i_idx;
    int i_prog;
    struct hazelnut_script_object *obj;


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

        idx = script->internal_counter.header_index;
        progress = script->internal_counter.header_progress;

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
                for (i = 0; i < script->header.key_count; i++) {
                    if (memcmp(script->header.objects[i].key, "unixtime", 8) == 0) {
                        val = script->header.objects[i].value;
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
            i_idx = script->internal_counter.instruction_index;
            i_prog = script->internal_counter.instruction_progress;

            obj = &script->objects[i_idx];

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
    int total_instructions;
    int i;
    if (script == NULL || script->objects == NULL) return NULL;

    // Search through instruction_index (or instruction count)
     total_instructions = script->internal_counter.instruction_index;

    for (i = 0; i < total_instructions; i++) {
        if (script->objects[i].address == target_address) {
            return &script->objects[i]; // Return pointer to matching object
        }
    }

    return NULL; // Address not found
}

int hazelnut_get_memory(struct hazelnut_script *script, int address) {
    return script->memory.memory[address];
}

int hazelnut_tick_script(struct hazelnut_script *script) {
    struct hazelnut_script_object *instruction_object;
    char opcode_str[3];
    int data;

    if (!script) return 1;

    instruction_object = get_instruction_by_address(script, script->program_counter);

    if (instruction_object == NULL) {
        //hvm_print("Error: No instruction found at address ");
        //hvm_print_int(&script->program_counter);
        //hvm_print("\n\r");

        // Advance PC or halt execution to break infinite loop
        script->program_counter++;
        return -1;
    }

    opcode_str[0] = (char)(instruction_object->opcode >> 8);
    opcode_str[1] = (char)(instruction_object->opcode & 0xFF);
    opcode_str[2] = '\0';
    //hvm_print("\n\rCURRENT OPCODE: ");
    //hvm_print(opcode_str);

    if (strcmp(opcode_str, "st") == 0) {
        //STORE VALUE IN MEMORY
        //hvm_print("\n\r");
        //hvm_print("Storing value ");
        //hvm_print_int(&instruction_object->operand2);
        //hvm_print("At location: ");
        //hvm_print_int(&instruction_object->operand1);
        //hvm_print("\n\r");
        hazelnut_set_memory(script, instruction_object->operand1, instruction_object->operand2);
    }
    if (strcmp(opcode_str, "pr") == 0) {
        //hvm_print("\n\r");
        //hvm_print("Printing value at");
        //hvm_print_int(&instruction_object->operand1);
        data = hazelnut_get_memory(script,instruction_object->operand1);
        hvm_print_int(&data);
        hvm_print("\n\r");
        //hazelnut_get_memory(script, instruction_object);
    }

    //hvm_print("\r");

    //hvm_print("\n\rEnd of instruction \n\r");
    //hvm_print("\n\rCURRENT PARAM1: \n\r");
    //hvm_print_int(&instruction_object->operand1);
    //hvm_print("\n\r");

    //hvm_print("\n\rCURRENT PARAM2: \n\r");
    //hvm_print_int(&instruction_object->operand2);
    //hvm_print("\n\r");

    script->program_counter++;

    return 0;
}

void hazlenut_run_file(int *s){

    unsigned char *byte_ptr;
    unsigned char current_byte;
    char buf[8];
    struct hazelnut_script hazelnut_script = {0};;

    if (s == NULL) {
        hvm_print("Error: Invalid or uninitialized file pointer.\n");
        return;
    }

    // Cast the start pointer to inspect memory as raw 8-bit bytes
    byte_ptr = (unsigned char *)s;

    // Loop until we hit the -1 terminator in 32-bit integer form




    while (*(int *)byte_ptr != -1) {
        current_byte = *byte_ptr;

        // Process individual byte (e.g., format and print)
        buf[8];
        #ifndef __CC65__
            snprintf(buf, sizeof(buf), "0x%02X ", current_byte);
        #endif
        //hvm_print(buf);
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
