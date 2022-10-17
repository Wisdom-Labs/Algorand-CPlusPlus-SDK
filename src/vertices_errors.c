/*
 * Copyright (c) 2021 Vertices Network <cyril@vertices.network>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <vertices_log.h>
#include <stdint.h>
#include "vertices_errors.h"
#include "compilers.h"

#if defined _WIN32 || defined _WIN64
#define APPERROR_EXPORT __declspec(dllexport)
#endif

#ifndef EXAMPLELIBRARY_EXPORT
#define EXAMPLELIBRARY_EXPORT
#endif

/**
 * Function is implemented as weak so that it can be overwritten by custom application error handler
 * when needed.
 */
// __WEAK
void
app_error_fault_handler(uint32_t id, uint32_t pc, long info)
{
    UNUSED_PARAMETER(id);
    UNUSED_PARAMETER(pc);

    LOG_ERROR("Fatal error: 0x%x %s:%u",
              ((error_info_t *) info)->err_code,
              ((error_info_t *) info)->p_file_name,
              ((error_info_t *) info)->line_num);
    // software breakpoint
    // __builtin_trap();
}

APPERROR_EXPORT void
app_error_handler(ret_code_t error_code, uint32_t line_num, const uint8_t *p_file_name)
{
    error_info_t error_info = {
        .line_num    = line_num,
        .p_file_name = p_file_name,
        .err_code    = error_code,
    };
    app_error_fault_handler(0, 0, (long) (&error_info));
}
