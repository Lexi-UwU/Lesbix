//
// Created by Lexi on 24/07/2026.
//

#ifndef LESBIX_DRIVERRESPONSE_H
#define LESBIX_DRIVERRESPONSE_H


#include <stddef.h>

typedef struct {
    char *data_char;
    int *data_int;
    size_t size;
    int response_code;
} DriverResponse;

#endif //LESBIX_DRIVERRESPONSE_H
