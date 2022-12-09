// Adaptation of lv2 books by lherg for the tutorial:
// https://github.com/lherg/tutos.ui.lv2 
// Copyright 2011-2016 David Robillard <d@drobilla.net>
// SPDX-License-Identifier: ISC

#include "lv2/core/lv2.h"
#include "lv2/core/lv2_util.h"
#include "lv2/ui/ui.h"

#include <gtk/gtk.h>
//#include <iostream>

#define AMP_UI_URI "http://lv2plug.in/plugins/eg-amp-gtk2#ui"

typedef struct {
  LV2UI_Write_Function write;
  LV2UI_Controller     controller;

  // Optional show/hide interface
  GtkWidget* window;
  GtkWidget* box;
  GtkWidget* hscale;   
  GtkObject* adjustment;
  GtkWidget* label;

  bool       did_init;
} AmpUI;

/** Send current UI settings to backend. */
static void
send_ui_state(LV2UI_Handle handle)
{
  AmpUI*  ui   = (AmpUI*)handle;
  const float gain = (float)gtk_range_get_value(GTK_RANGE(ui->hscale ));
  //std::cout << "UI/send_ui_state/gain:" << gain << "\n";
  ui->write (ui->controller, 0, sizeof (float), 0, &gain);

}

/** Gtk widget callback. */
static gboolean
on_cfg_changed(GtkWidget* widget, gpointer data)
{
  AmpUI* ui = (AmpUI*)data;
  send_ui_state(data);

  return TRUE;
}

static void
destroy_window(AmpUI* ui)
{
  if (ui->window) {
    gtk_container_remove(GTK_CONTAINER(ui->window), ui->box);
    gtk_widget_destroy(ui->window);
    ui->window = NULL;
  }
}

static gboolean
on_window_closed(GtkWidget* widget, GdkEvent* event, gpointer data)
{
  AmpUI* ui = (AmpUI*)data;

  // Remove widget so Gtk doesn't delete it when the window is closed
  gtk_container_remove(GTK_CONTAINER(ui->window), ui->box);
  ui->window = NULL;

  return FALSE;
}

static LV2UI_Handle
instantiate(const LV2UI_Descriptor*   descriptor,
            const char*               plugin_uri,
            const char*               bundle_path,
            LV2UI_Write_Function      write_function,
            LV2UI_Controller          controller,
            LV2UI_Widget*             widget,
            const LV2_Feature* const* features)
{
  AmpUI* ui = (AmpUI*)calloc(1, sizeof(AmpUI));
  if (!ui) {
    return NULL;
  }

  ui->write      = write_function;
  ui->controller = controller;
  *widget        = NULL;
  ui->window     = NULL;
  ui->did_init   = false;

  // Construct Gtk UI

  ui->box = gtk_hbox_new(FALSE, 4);
  
  ui->adjustment = gtk_adjustment_new (0.0, -90.0, 24.0, 0.1, 5.0, 0.0);
  ui->hscale      = gtk_hscale_new((GtkAdjustment *)ui->adjustment );
  ui->label = gtk_label_new("Gain (dB): ");
  gtk_widget_set_size_request(ui->hscale, 620, 60);

  gtk_container_set_border_width(GTK_CONTAINER(ui->box), 4);
  
  gtk_box_pack_start(GTK_BOX(ui->box), ui->label, TRUE, TRUE, 0); 
  gtk_box_pack_start(GTK_BOX(ui->box), ui->hscale, TRUE, TRUE, 0);
  
  g_signal_connect(
    G_OBJECT(ui->hscale), "value-changed", G_CALLBACK(on_cfg_changed), ui);

  *widget = ui->box;

  return ui;
}

static void
cleanup(LV2UI_Handle handle)
{
  AmpUI* ui = (AmpUI*)handle;

  if (ui->window) {
    destroy_window(ui);
  }
  g_object_unref(ui->adjustment);
  gtk_widget_destroy(ui->hscale);
  gtk_widget_destroy(ui->box);
  free(ui);
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
	if (port_index == 0) {
    const float _gain       = *(float*)buffer;
    gtk_range_set_value (GTK_RANGE(ui->hscale ),_gain);

	}  
}

/* Optional non-embedded UI show interface. */
static int
ui_show(LV2UI_Handle handle)
{
  AmpUI* ui = (AmpUI*)handle;

  if (ui->window) {
    return 0;
  }

  if (!ui->did_init) {
    int argc = 0;
    gtk_init_check(&argc, NULL);
    g_object_ref(ui->box);
    ui->did_init = true;
  }
  ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //gtk_window_set_title(GTK_WINDOW(ui->window), "Glh-AmP");
  //gtk_window_set_default_size(GTK_WINDOW(ui->window ), 600, 200);
  //gtk_container_set_border_width(GTK_CONTAINER(ui->window ), 4); 

  gtk_container_add(GTK_CONTAINER(ui->window), ui->box);

  g_signal_connect(
    G_OBJECT(ui->window), "delete-event", G_CALLBACK(on_window_closed), handle);

  gtk_widget_show_all(ui->window);

  gtk_window_present(GTK_WINDOW(ui->window));

  return 0;
}

/* Optional non-embedded UI hide interface. */
static int
ui_hide(LV2UI_Handle handle)
{
  AmpUI* ui = (AmpUI*)handle;

  if (ui->window) {
    destroy_window(ui);
  }

  return 0;
}

/* Idle interface for optional non-embedded UI. */
static int
ui_idle(LV2UI_Handle handle)
{
  AmpUI* ui = (AmpUI*)handle;
  if (ui->window) {
    gtk_main_iteration_do(false);
  }
  return 0;
}

static const void*
extension_data(const char* uri)
{
  static const LV2UI_Show_Interface show = {ui_show, ui_hide};
  static const LV2UI_Idle_Interface idle = {ui_idle};

  if (!strcmp(uri, LV2_UI__showInterface)) {
    return &show;
  }

  if (!strcmp(uri, LV2_UI__idleInterface)) {
    return &idle;
  }

  return NULL;
}

static const LV2UI_Descriptor descriptor = {AMP_UI_URI,
                                            instantiate,
                                            cleanup,
                                            port_event,
                                            extension_data};

LV2_SYMBOL_EXPORT
const LV2UI_Descriptor*
lv2ui_descriptor(uint32_t index)
{
  return index == 0 ? &descriptor : NULL;
}
