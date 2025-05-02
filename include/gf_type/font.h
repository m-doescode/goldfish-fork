/**
 * @file gf_type/font.h
 * @~english
 * @brief Type definitions related to font
 */

#ifndef __GF_TYPE_FONT_H__
#define __GF_TYPE_FONT_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_FONT
typedef struct gf_font_bbox_t  gf_font_bbox_t;
typedef struct gf_font_glyph_t gf_font_glyph_t;
typedef struct gf_font_t       gf_font_t;

/* External library */

/* Engine */
#include <gf_type/texture.h>

/* Standard */

/**
 * @struct gf_font_store_t
 * @~english
 * @brief Internal struct used for parsing
 *
 * @var gf_font_store_t::glyph_index
 * @brief Current glyph index
 *
 * @var gf_font_store_t::glyph_index
 * @brief Current line index of glyph
 *
 * @var gf_font_store_t::buffer
 * @brief Glyph buffer
 */
GF_DECLARE_TYPE(font_store, {
	int	       glyph_index;
	int	       line_index;
	unsigned char* buffer;
});

/**
 * @struct gf_font_bbox_t
 * @~english
 * @brief Bounding box
 *
 * @var gf_font_bbox_t::width
 * @brief Width
 *
 * @var gf_font_bbox_t::height
 * @brief Height
 *
 * @var gf_font_bbox_t::x
 * @brief X coord
 *
 * @var gf_font_bbox_t::y
 * @brief Y coord
 */
GF_DECLARE_TYPE(font_bbox, {
	int width;
	int height;
	int x;
	int y;
});

/**
 * @struct gf_font_glyph_t
 * @~english
 * @brief Glyph
 *
 * @var gf_font_glyph_t::code
 * @brief Character code
 *
 * @var gf_font_glyph_t::texture
 * @brief Texture
 *
 * @var gf_font_glyph_t::bpl
 * @brief Bytes per line
 *
 * @var gf_font_glyph_t::bbox
 * @brief Bounding box
 *
 * @var gf_font_glyph_t::dwidth
 * @brief Device width
 */
GF_DECLARE_TYPE(font_glyph, {
	int	       code;
	gf_texture_t*  texture;
	int	       bpl;
	gf_font_bbox_t bbox;
	int	       dwidth[2];
});

/**
 * @struct gf_font_t
 * @~english
 * @brief Font
 *
 * @var gf_font_t::glyph
 * @brief Glyph data
 *
 * @var gf_font_t::count
 * @brief Glyph count
 *
 * @var gf_font_t::bbox
 * @brief Bounding box
 */
GF_DECLARE_TYPE(font, {
	gf_font_glyph_t** glyph;
	int		  count;
	gf_font_bbox_t	  bbox;
});
#else
typedef void gf_font_glyph_t;
typedef void gf_font_bbox_t;
typedef void gf_font_t;
#endif

#endif
