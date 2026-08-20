#define HVM_CORE

#ifndef HVM_ABSTRACTION
    #include "hazelnut_out.h"
#endif

#include <stdio.h>

#include "../tools/utils.h"

#include "hazelnut_objects.h"



#include <stdlib.h>

void hazelnut_proccess_byte(unsigned int byte, struct hazelnut_script *script) {
    unsigned int count = script->internal_counter.byte_count;

    // 1. Signature check
    if (count >= 8) {
        script->internal_counter.passed_identifier = 1;
    }

    // 2. Accumulate header length value (Bytes 24 through 31)
    if (count >= 24 && count < 32) {
        script->header.key_count = (script->header.key_count << 8) | (byte & 0xFF);
    }

    // 3. Mark length header parsed at byte 31
    if (count >= 31) {
        script->internal_counter.passed_headersize = 1;
    }

    // --- STATE MACHINE ---

    if (script->internal_counter.passed_identifier == 0) {
        /* Bytes 0 - 7: Magic Signature */
    }
    else if (script->internal_counter.passed_headersize == 0) {
        /* Bytes 8 - 31: Padding & Header Size Entry */
    }
    else {
        /* Bytes 32+: Header Entries & Instructions */

        // LAZY ALLOCATION GUARD: Allocate on the first byte of the header section (Byte 32)
        if (script->header.objects == NULL && script->header.key_count > 0) {
            script->header.objects = calloc(script->header.key_count, sizeof(struct hazelnut_script_header_object));
            script->internal_counter.header_index = 0;
            script->internal_counter.header_progress = 0;
        }

        unsigned int idx = script->internal_counter.header_index;
        unsigned int progress = script->internal_counter.header_progress;

        // CRITICAL SAFETY CHECK: Ensure array is allocated AND index is within bounds
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
        }
    }

    script->internal_counter.byte_count++;
}



void hazlenut_run_file(int *s){

    if (s == NULL) {
        hvm_print("Error: Invalid or uninitialized file pointer.\n");
        return;
    }

    // Cast the start pointer to inspect memory as raw 8-bit bytes
    unsigned char *byte_ptr = (unsigned char *)s;

    // Loop until we hit the -1 terminator in 32-bit integer form

    struct hazelnut_script hazelnut_script;


    while (*(int *)byte_ptr != -1) {
        unsigned char current_byte = *byte_ptr;

        // Process individual byte (e.g., format and print)
        char buf[8];
        snprintf(buf, sizeof(buf), "0x%02X ", current_byte);
        hvm_print(buf);
        hazelnut_proccess_byte(current_byte,&hazelnut_script);

        byte_ptr++; // Advance to the next 8-bit byte
    }

    hvm_print("\n\r");
}
