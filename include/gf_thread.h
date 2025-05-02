/**
 * @file gf_thread.h
 * @~english
 * @brief Thread interface
 */

#ifndef __GF_THREAD_H__
#define __GF_THREAD_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/thread.h>

/* Engine */

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create thread
 * @param func Function to be called for thread
 * @param userdata Data to be passed to thread
 * @return Thread
 */
GF_EXPORT gf_thread_t* gf_thread_create(void (*func)(void*), void* userdata);

/**
 * @~english
 * @brief Join thread
 * @param thread Thread
 */
GF_EXPORT void gf_thread_join(gf_thread_t* thread);

/**
 * @~english
 * @brief Destroy thread
 * @param thread Thread
 */
GF_EXPORT void gf_thread_destroy(gf_thread_t* thread);

#ifdef __cplusplus
}
#endif

#endif
