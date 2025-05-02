#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_gui.h>

/* Engine */
#include <gf_prop.h>
#include <gf_graphic.h>
#include <gf_draw.h>
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_button(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t c;

	gf_gui_create_component(gui, &c, x, y, w, h);

	c.type = GF_GUI_BUTTON;

	hmputs(gui->area, c);

	return c.key;
}

void gf_gui_button_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	   input = gui->draw->input;
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	double		   x;
	double		   y;
	gf_prop_integer_t  prop;
	gf_prop_floating_t propf;
	gf_font_t*	   font;
	if(c->type != GF_GUI_BUTTON) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	if((prop = gf_prop_get_integer(&c->prop, "no-border")) == GF_PROP_NO_SUCH || !prop) {
		gf_gui_draw_box(gui, (gui->pressed == c->key) ? GF_GUI_INVERT : GF_GUI_NORMAL, cx, cy, cw, ch);
	}

	if(c->text != NULL) {
		double fsz = GF_GUI_SMALL_FONT_SIZE;
		double ogx;
		double ogy;
		if((propf = gf_prop_get_floating(&c->prop, "font-size")) != GF_PROP_NO_SUCH) {
			fsz = propf;
		}
		if((prop = gf_prop_get_integer(&c->prop, "align")) == GF_PROP_NO_SUCH || prop == 0) {
			x = cx + cw / 2 - gf_graphic_text_width(gui->draw, font, fsz, c->text) / 2;
			y = cy + ch / 2 - (double)fsz / 2;
		} else if(prop == -1) {
			x = cx;
			y = cy + ch / 2 - (double)fsz / 2;
		} else if(prop == 1) {
			x = cx - gf_graphic_text_width(gui->draw, font, fsz, c->text);
			y = cy + ch / 2 - (double)fsz / 2;
		}

		ogx = x;
		ogy = y;
		ogx += gf_gui_border_width / 2;
		ogy += gf_gui_border_width / 2;

		if(gui->pressed == c->key) {
			x += gf_gui_border_width / 2;
			y += gf_gui_border_width / 2;
		}

		gf_graphic_clip_push(gui->draw, cx, cy, cw, ch);
		if((prop = gf_prop_get_integer(&c->prop, "no-border")) != GF_PROP_NO_SUCH && prop) {
			gf_graphic_color_t dim;
			dim.r = 0;
			dim.g = 0;
			dim.b = 0;
			dim.a = 128;
			gf_graphic_text(gui->draw, font, ogx, ogy, fsz, c->text, dim);
		}
		if(gui->hover == c->key) {
			gf_graphic_text(gui->draw, font, x, y, fsz, c->text, c->hover_font);
		} else {
			gf_graphic_text(gui->draw, font, x, y, fsz, c->text, c->font);
		}
		gf_graphic_clip_pop(gui->draw);
	}
}

void gf_gui_button_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_BUTTON) return;
}

void gf_gui_button_click(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	  input = gui->draw->input;
	gf_prop_integer_t prop;
	if(c->type != GF_GUI_BUTTON) return;
	if(c->callback != NULL) {
		c->callback(gui->engine, gui->draw, gui->pressed, GF_GUI_PRESS_EVENT);
	}
	c->pressed = 1;
	if((prop = gf_prop_get_integer(&c->prop, "close-parent")) != GF_PROP_NO_SUCH && prop) {
		gf_gui_destroy_id(gui, c->parent);
	}
}
