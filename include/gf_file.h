/**
 * @file gf_file.h
 * @~english
 * @brief File I/O
 */

#ifndef __GF_FILE_H__
#define __GF_FILE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/file.h>

/* Engine */
#include <gf_type/core.h>

/* Standard */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Open file
 * @param engine Engine instance
 * @param path Path
 * @param mode Mode
 * @return File
 */
GF_EXPORT gf_file_t* gf_file_open(gf_engine_t* engine, const char* path, const char* mode);

/**
 * @~english
 * @brief Read file
 * @param fp File
 * @param buffer Buffer
 * @param size Size
 * @return Read size
 */
GF_EXPORT size_t gf_file_read(gf_file_t* fp, void* buffer, size_t size);

/**
 * @~english
 * @brief Write to file
 * @param fp File
 * @param buffer Buffer
 * @param size Size
 * @return Written size
 */
GF_EXPORT size_t gf_file_write(gf_file_t* fp, void* buffer, size_t size);

/**
 * @~english
 * @brief Close file
 * @param fp File
 */
GF_EXPORT void gf_file_close(gf_file_t* fp);

#ifdef __cplusplus
}
#endif

#endif
