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

typedef struct {
	LV2UI_Write_Function write;
	LV2UI_Controller     controller;
	LV2UI_Touch*         touch;

	RobWidget* rw;
	RobWidget* amp_table;
	RobTkDial* amp_gain;
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
	ui->amp_gain->displaymode = 3;
	if (ui->touch) {
		robtk_dial_set_touch (ui->amp_gain, ui->touch->touch, ui->touch->handle, AMP_OUTPUT);
	}
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

