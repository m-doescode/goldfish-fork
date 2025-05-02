/**
 * @file gf_type/thread.h
 * @~english
 * @brief Type definitions related to thread interface
 */

#ifndef __GF_TYPE_THREAD_H__
#define __GF_TYPE_THREAD_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_THREAD
typedef struct gf_thread_context_t gf_thread_context_t;
typedef struct gf_thread_t	   gf_thread_t;

/* External library */
#if defined(THREAD_POSIX)
#include <pthread.h>
#elif defined(THREAD_WIN32)
#include <windows.h>
#endif

/* Engine */

/* Standard */

/**
 * @struct gf_thread_context_t
 * @~english
 * @brief Thread context
 *
 * @var gf_thread_context_t::func
 * @brief Function to be called for thread
 *
 * @var gf_thread_context_t::data
 * @brief Data to be passed to thread
 */
GF_DECLARE_TYPE(thread_context, {
	void (*func)(void*);
	void* data;
});

#if defined(THREAD_POSIX)
GF_DECLARE_TYPE(thread, {
	gf_thread_context_t context;
	pthread_t	    thread;
});
#elif defined(THREAD_WIN32)
GF_DECLARE_TYPE(thread, {
	gf_thread_context_t context;
	HANDLE		    thread;
});
#else
/**
 * @struct gf_thread_t
 * @~english
 * @brief Platform-dependent thread
 * @note Definition depends on the platform, read the header file for definition
 */
GF_DECLARE_TYPE(thread, {});
#endif
#else
typedef void gf_thread_t;
typedef void gf_thread_context_t;
#endif

#endif
