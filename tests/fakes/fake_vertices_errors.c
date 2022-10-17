//
// Created by Cyril on 09/04/2021.
//

#include <vertices_log.h>
#include <stdint.h>
#include "vertices_errors.h"
#include "compilers.h"

/**
 * Function is implemented as weak so that it can be overwritten by custom application error handler
 * when needed.
 */
__WEAK void
app_error_fault_handler(__attribute__((unused)) uint32_t id,
                        __attribute__((unused)) uint32_t pc,
                        long info)
{

    LOG_ERROR("Fatal error: 0x%x %s:%u",
              ((error_info_t *) info)->err_code,
              ((error_info_t *) info)->p_file_name,
              ((error_info_t *) info)->line_num);
    // software breakpoint
    // I got this error so remove this line.
    // error LNK2019: unresolved external symbol __builtin_trap referenced in function app_error_fault_handler when Linking bin/vertices.dll with lib/vertices.lib
    //__builtin_trap();
}

void
app_error_handler(ret_code_t error_code, uint32_t line_num, const uint8_t *p_file_name)
{
    error_info_t error_info = {
        .line_num    = line_num,
        .p_file_name = p_file_name,
        .err_code    = error_code,
    };
    app_error_fault_handler(0, 0, (long) (&error_info));
}
