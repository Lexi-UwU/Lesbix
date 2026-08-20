#define HVM_CORE

#ifndef HVM_ABSTRACTION
    #include "hazelnut_out.h"
#endif

#include "../tools/utils.h"


void hazlenut_run_file(int *s){

    //Initialise headers and load some object thing into memory

    //Run in a loop

    if (s == NULL) {
        hvm_print("Error: Invalid or uninitialized file pointer.\n");
        return;
    }

    for (int *ptr = s; *ptr != -1; ptr++) {
        int current_val = *ptr;



        hvm_print_int(&current_val);


        // Process each integer
    }



}
