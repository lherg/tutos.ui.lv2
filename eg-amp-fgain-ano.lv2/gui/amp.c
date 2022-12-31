// https://github.com/lherg/tutos.ui.lv2 
// eg-amp-robtk.lv2
// gui/amp.c

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include "../src/amp.h"

#define RTK_URI AMP_URI
#define RTK_GUI "ui"

//#define GD_W  140
//#define GD_H  80
//#define GD_RAD 30
//#define GD_CX 70
//#define GD_CY 40

typedef struct {
	LV2UI_Write_Function write;
	LV2UI_Controller     controller;
	LV2UI_Touch*         touch;

	PangoFontDescription* font[2];

	RobWidget* rw;
	RobWidget* amp_table;

	RobTkDial* amp_gain;
	cairo_surface_t* amp_cr;

	RobTkLbl*  amp_lbl;

	bool disable_signals;
} AmpUI;

/*****************************************************************************/
// UI

/* Remove an delete UI windows. */
static void
destroy_window(AmpUI* ui)
{
    robtk_dial_destroy (ui->amp_gain);
    robtk_lbl_destroy (ui->amp_lbl);
    rob_table_destroy (ui->amp_table);
    rob_box_destroy (ui->rw);
}

static void
prepare_faceplates (AmpUI* ui)
{
	cairo_t* cr;
	float    xlp, ylp;

/* clang-format off */
#define INIT_DIAL_SF(VAR, W, H)                                             \
  VAR = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 2 * (W), 2 * (H)); \
  cr  = cairo_create (VAR);                                                 \
  cairo_scale (cr, 2.0, 2.0);                                               \
  CairoSetSouerceRGBA (c_trs);                                              \
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);                           \
  cairo_rectangle (cr, 0, 0, W, H);                                         \
  cairo_fill (cr);                                                          \
  cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

#define DIALDOTS(V, XADD, YADD)                                \
  float ang = (-.75 * M_PI) + (1.5 * M_PI) * (V);              \
  xlp       = GED_CX + XADD + sinf (ang) * (GED_RADIUS + 3.0); \
  ylp       = GED_CY + YADD - cosf (ang) * (GED_RADIUS + 3.0); \
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);               \
  CairoSetSouerceRGBA (c_g80);                                 \
  cairo_set_line_width (cr, 2.5);                              \
  cairo_move_to (cr, rint (xlp) - .5, rint (ylp) - .5);        \
  cairo_close_path (cr);                                       \
  cairo_stroke (cr);

#define RESPLABLEL(V)                                             \
  {                                                               \
    DIALDOTS (V, 4.5, 15.5)                                       \
    xlp = rint (GED_CX + 4.5 + sinf (ang) * (GED_RADIUS + 9.5));  \
    ylp = rint (GED_CY + 15.5 - cosf (ang) * (GED_RADIUS + 9.5)); \
  }
  
	
	INIT_DIAL_SF (ui->amp_cr, GED_WIDTH + 8, GED_HEIGHT + 20);
	RESPLABLEL (0.00);
	write_text_full (cr, "-12", ui->font[0], xlp + 6, ylp, 0, 1, c_g80);
	RESPLABLEL (0.25);
	RESPLABLEL (0.5);
	write_text_full (cr, "0", ui->font[0], xlp, ylp, 0, 2, c_g80);
	RESPLABLEL (.75);
	RESPLABLEL (1.0);
	write_text_full (cr, "+12", ui->font[0], xlp - 6, ylp, 0, 3, c_g80);
	cairo_destroy (cr);

#undef DIALDOTS
#undef INIT_DIAL_SF
#undef RESPLABLEL
}

static void
display_annotation (AmpUI* ui, RobTkDial* d, cairo_t* cr, const char* txt)
{
	int tw, th;
	cairo_save (cr);
	PangoLayout* pl = pango_cairo_create_layout (cr);
	pango_layout_set_font_description (pl, ui->font[0]);
	pango_layout_set_text (pl, txt, -1);
	pango_layout_get_pixel_size (pl, &tw, &th);
	cairo_translate (cr, d->w_width / 2, d->w_height - 2);
	cairo_translate (cr, -tw / 2.0, -th);
	cairo_set_source_rgba (cr, .0, .0, .0, .7);
	rounded_rectangle (cr, -1, -1, tw + 3, th + 1, 3);
	cairo_fill (cr);
	CairoSetSouerceRGBA (c_wht);
	pango_cairo_show_layout (cr, pl);
	g_object_unref (pl);
	cairo_restore (cr);
	cairo_new_path (cr);
}

static void
dial_annotation_db (RobTkDial* d, cairo_t* cr, void* data)
{
	AmpUI* ui = (AmpUI*)(data);
	char    txt[16];
	snprintf (txt, 16, "%5.1f dB", d->cur);
	display_annotation (ui, d, cr, txt);
}

/* Gain callback (Gtk widget). */
static bool
cb_amp_gain(RobWidget* w, void* handle)
{
	AmpUI* ui = (AmpUI*)handle;
	float gain = robtk_dial_get_value (ui->amp_gain);
	if (ui->disable_signals) {
		return TRUE;
	}
	ui->write (ui->controller, AMP_GAIN, sizeof (float), 0, &gain);
	return TRUE;
}

static RobWidget*
toplevel (AmpUI* ui, void* const top)
{
	/* toplevel */
	ui->rw = rob_vbox_new (FALSE, 2);
	robwidget_make_toplevel (ui->rw, top);
	robwidget_toplevel_enable_scaling (ui->rw);

	ui->font[0] = pango_font_description_from_string ("Mono 9px");
	ui->font[1] = pango_font_description_from_string ("Mono 10px");

	prepare_faceplates (ui);

	/* toplevel */
	ui->amp_table = rob_table_new (/*rows*/ 2, /*cols*/ 1, FALSE);

	/* label */
	ui->amp_lbl = robtk_lbl_new ("Gain");
	rob_table_attach (ui->amp_table, robtk_lbl_widget (ui->amp_lbl),
	                  1, 2,
	                  0, 1,
	                  2, 2, RTK_SHRINK, RTK_SHRINK);


	/* dial */
	ui->amp_gain = robtk_dial_new_with_size (
		-12.0f, 12.0f, 0.1f,
		GED_WIDTH + 8, GED_HEIGHT + 20, GED_CX + 4, GED_CY + 15, GED_RADIUS);
		
	robtk_dial_set_default (ui->amp_gain, 0.0f);
	robtk_dial_set_callback (ui->amp_gain, cb_amp_gain, ui);
	float detent = 0.f;
	robtk_dial_set_detents (ui->amp_gain, 6 , &detent);
	robtk_dial_enable_states (ui->amp_gain, 1);
	robtk_dial_set_state_color (ui->amp_gain, 1, 1.0, .0, .0, .3);
	robtk_dial_set_default_state (ui->amp_gain, 0);

	robtk_dial_annotation_callback (ui->amp_gain, dial_annotation_db, ui);	

	ui->amp_gain->displaymode = 3;

	if (ui->touch) {
		robtk_dial_set_touch (ui->amp_gain, ui->touch->touch, ui->touch->handle, AMP_OUTPUT);
	}

	robtk_dial_set_scaled_surface_scale (ui->amp_gain, ui->amp_cr, 2.0);
	
	rob_table_attach (ui->amp_table, robtk_dial_widget (ui->amp_gain),
						1, 2,
						1, 2,
						0, 0, RTK_SHRINK, RTK_SHRINK);

	/* top-level packing */
	rob_vbox_child_pack (ui->rw, ui->amp_table, TRUE, TRUE);
	return ui->rw;
}

/*****************************************************************************/
// LV2

#define LVGL_RESIZEABLE
static void ui_disable(LV2UI_Handle handle) { }
static void ui_enable(LV2UI_Handle handle) { }

static LV2UI_Handle
instantiate (
    void* const               ui_toplevel,
    const LV2UI_Descriptor*   descriptor,
    const char*               plugin_uri,
    const char*               bundle_path,
    LV2UI_Write_Function      write_function,
    LV2UI_Controller          controller,
    RobWidget**               widget,
    const LV2_Feature* const* features)
{
	AmpUI* ui = (AmpUI*)calloc (1, sizeof (AmpUI));
	if (!ui) {
		return NULL;
	}

	for (int i = 0; features[i]; ++i) {
		if (!strcmp (features[i]->URI, LV2_UI__touch)) {
			ui->touch = (LV2UI_Touch*)features[i]->data;
		}
	}

	ui->write           = write_function;
	ui->controller      = controller;
	ui->disable_signals = true;

	*widget             = toplevel (ui, ui_toplevel);
	ui->disable_signals = false;
	return ui;
}

static enum LVGLResize
plugin_scale_mode (LV2UI_Handle handle)
{
	return LVGL_LAYOUT_TO_FIT;
}

static void
cleanup(LV2UI_Handle handle)
{
	AmpUI* ui = (AmpUI*)handle;
	destroy_window (ui);
	free (ui);
}

/* receive information from DSP */
static void
port_event(LV2UI_Handle handle,
            uint32_t     port_index,
            uint32_t     buffer_size,
            uint32_t     format,
            const void*  buffer)
{
	AmpUI* ui = (AmpUI*)handle;
	if (port_index == AMP_GAIN) {
    const float _gain = *(float*)buffer;
    ui->disable_signals = true;
    robtk_dial_set_value (ui->amp_gain, _gain);
    ui->disable_signals = false;
	}
}

static const void*
extension_data(const char* uri)
{
	return NULL;
}

