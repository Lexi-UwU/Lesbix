//
// Created by Lexi on 20/08/2026.
//

#ifndef LESBIX_HAZELNUT_OBJECTS_H
#define LESBIX_HAZELNUT_OBJECTS_H

struct hazelnut_script_header_object {
    int *key;
    int *value;
};

struct hazelnut_script_header {
    int key_count;
    struct hazelnut_script_header_object *objects;
};

struct hazelnut_script_parse_internal_counter {
    int byte_count;
    int passed_identifier;
    int passed_headersize;
    int header_index;
    int header_progress;
};

struct hazelnut_script_object {
    int address;
    int opcode;
    int operand1;
    int operand2;
    int debug;
};


struct hazelnut_script {
    struct hazelnut_script_header header;
    struct hazelnut_script_object *objects;
    struct hazelnut_script_parse_internal_counter internal_counter;;
};


#endif //LESBIX_HAZELNUT_OBJECTS_H
