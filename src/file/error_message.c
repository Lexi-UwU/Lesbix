//
// Created by Lexi on 26/07/2026.
//

#include "error_message.h"
#include "driver_response.h"
#include "../tools/utils.h"



DriverResponse FILESYSTEM_CREATE_ERROR_RESPONSE(const char *err_msg, int code) {
    DriverResponse resp;
    resp.size = strlen(err_msg);
    resp.data_char = malloc(resp.size + 1);
    if (resp.data_char) {
        strcpy(resp.data_char, err_msg);
    }
    resp.data_int = NULL;
    resp.response_code = code;
    return resp;
}
