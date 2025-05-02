#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_log.h>

/* Engine */
#include <gf_core.h>

/* Standard */
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

FILE* gf_log_default = NULL;

void gf_log(gf_engine_t* engine, const char* fmt, ...) {
	va_list va;
	FILE*	out = gf_log_default;
	if(engine != NULL) {
		out = engine->log;
	}
	if(out != NULL) {
		va_start(va, fmt);
		vfprintf(out, fmt, va);
		va_end(va);
	}
}
