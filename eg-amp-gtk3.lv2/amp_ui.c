// https://github.com/lherg/tutos.ui.lv2 
// eg-amp-gtk3.lv2
// amp_ui.c

#include "lv2/core/lv2.h"
#include "lv2/core/lv2_util.h"
#include "lv2/ui/ui.h"

#include <gtk/gtk.h>

#include "amp.h"

typedef struct {
  LV2UI_Write_Function write;
  LV2UI_Controller     controller;

  GtkWidget* window;
  GtkWidget* box;
  GtkWidget* hscale;   
  GtkAdjustment* adjustment;
  GtkWidget* label;

  bool       did_init;
} AmpUI;

/*****************************************************************************/
// UI

/* Remove an delete UI windows. */
static void
destroy_window(AmpUI* ui)
{
  if (ui->window) {
    gtk_container_remove(GTK_CONTAINER(ui->window), ui->box);
    gtk_widget_destroy(ui->window);
    ui->window = NULL;
  }
}

/* Remove widget so Gtk doesn't delete it when the window is closed */
static gboolean
on_window_closed(GtkWidget* widget, GdkEvent* event, gpointer data)
{
  AmpUI* ui = (AmpUI*)data;

  gtk_container_remove(GTK_CONTAINER(ui->window), ui->box);
  ui->window = NULL;

  return FALSE;
}

/* Gain callback (Gtk widget). */
static gboolean
cb_amp_gain(GtkWidget* widget, gpointer data)
{
  AmpUI* ui = (AmpUI*)data;
  const float gain = (float)gtk_range_get_value(GTK_RANGE(ui->hscale ));
  ui->write (ui->controller, 0, sizeof (float), 0, &gain);
  return TRUE;
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
  gtk_container_add(GTK_CONTAINER(ui->window), ui->box);

  g_signal_connect(G_OBJECT(ui->window), "delete-event", G_CALLBACK(on_window_closed), handle);

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

/*****************************************************************************/
// LV2

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
  
  ui->adjustment = gtk_adjustment_new (0.0, -12.0, 12.0, 0.1, 0.0, 0.0);
  ui->hscale      = gtk_hscale_new((GtkAdjustment *)ui->adjustment );
  ui->label = gtk_label_new("Gain (dB): ");
  gtk_widget_set_size_request(ui->hscale, 620, 60);

  gtk_container_set_border_width(GTK_CONTAINER(ui->box), 4);
  
  gtk_box_pack_start(GTK_BOX(ui->box), ui->label, TRUE, TRUE, 0); 
  gtk_box_pack_start(GTK_BOX(ui->box), ui->hscale, TRUE, TRUE, 0);
  
  g_signal_connect(G_OBJECT(ui->hscale), "value-changed", G_CALLBACK(cb_amp_gain), ui);

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
	if (port_index == AMP_GAIN) {
    const float _gain       = *(float*)buffer;
    gtk_range_set_value (GTK_RANGE(ui->hscale ),_gain);

	}  
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
