#pragma once

#include "vertices_log.h"
#include "vertices_http.h"
#include "compilers.h"
#include "utils/http_weak.h"

#if defined _WIN32 || defined _WIN64
#define HTTP_WEAK_EXPORT __declspec(dllexport)
#endif

#ifndef HTTP_WEAK_EXPORT
#define HTTP_WEAK_EXPORT
#endif

/*
 * ⚠️ This file contains weak implementations of HTTP functions
 * It is provided so that the Vertices SDK can be compiled and linked as a static library.
 * For the Vertices SDK to work, the user **must** declare and define a strong implementation.
 * Examples of such implementations are available, see documentation at docs.vertices.network.
 */

HTTP_WEAK_EXPORT void ExampleLibraryFunction(){

}

HTTP_WEAK_EXPORT ret_code_t set_http_init(ret_code_t (*http_init_handler)(const provider_info_t* provider,
                                                                        size_t(*response_payload_cb)(char* chunk,
                                                                                                     size_t chunk_size))) {
    m_http_init_handler = http_init_handler;
    return VTC_SUCCESS;
}

HTTP_WEAK_EXPORT ret_code_t set_http_get(ret_code_t (*http_get_handler)(const provider_info_t *provider,
                                                                        const char *relative_path,
                                                                        const char *headers,
                                                                        uint32_t *response_code)) {
    m_http_get_handler = http_get_handler;
    return VTC_SUCCESS;
}

HTTP_WEAK_EXPORT ret_code_t set_http_post(ret_code_t (*http_post_handler)(const provider_info_t *provider,
                                                                         const char *relative_path,
                                                                         char *headers,
                                                                         const char *body,
                                                                         size_t body_size,
                                                                         uint32_t *response_code)) {
    m_http_post_handler = http_post_handler;
    return VTC_SUCCESS;
}

HTTP_WEAK_EXPORT ret_code_t set_http_close(void (*http_close_handler)(void)) {
    m_http_close_handler = http_close_handler;
    return VTC_SUCCESS;
}

 ret_code_t
http_init(const provider_info_t *provider,
          size_t (*response_payload_cb)(char *chunk,
                                        size_t chunk_size))
{
    if(m_http_init_handler == NULL) {
        LOG_ERROR("Weak implementation of http_init");
        return VTC_ERROR_NOT_FOUND;
    }

    return m_http_init_handler(provider, response_payload_cb);
}

ret_code_t
http_get(const provider_info_t *provider,
         const char *relative_path,
         const char *headers,
         uint32_t *response_code)
{
    if(m_http_get_handler == NULL) {
        LOG_ERROR("Weak implementation of http_get");
        return VTC_ERROR_NOT_FOUND;
    }
    return m_http_get_handler(provider, relative_path, headers, response_code);
}

ret_code_t
http_post(const provider_info_t *provider,
          const char *relative_path,
          char *headers,
          const char *body,
          size_t body_size,
          uint32_t *response_code)
{
    if(m_http_post_handler == NULL) {
        LOG_ERROR("Weak implementation of http_post");
        return VTC_ERROR_NOT_FOUND;
    }
    return m_http_post_handler(provider, relative_path, headers, body, body_size, response_code);
}

void
http_close(void)
{
    m_http_close_handler();
}
