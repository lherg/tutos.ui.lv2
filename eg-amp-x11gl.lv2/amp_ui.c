// Adaptation of lv2 books by lherg for the tutorial:
// https://github.com/lherg/tutos.ui.lv2 
// Copyright 2011-2016 David Robillard <d@drobilla.net>
// SPDX-License-Identifier: ISC

#include "pugl/cairo.h"
#include "pugl/pugl.h"

#include <cairo.h>

#include "lv2/core/lv2.h"
#include "lv2/core/lv2_util.h"
#include "lv2/ui/ui.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define AMP_UI_URI "http://lv2plug.in/plugins/eg-amp-x11gl#ui"

typedef struct {
  LV2UI_Write_Function write;
  LV2UI_Controller     controller;
  PuglNativeView parentWindow;
  bool       did_init;
} AmpUI;

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
  return ui;
}

static void
cleanup(LV2UI_Handle handle)
{
  AmpUI* ui = (AmpUI*)handle;

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

	}  
}

/* Optional non-embedded UI show interface. */
static int
ui_show(LV2UI_Handle handle)
{
  AmpUI* ui = (AmpUI*)handle;
  return 0;
}

/* Optional non-embedded UI hide interface. */
static int
ui_hide(LV2UI_Handle handle)
{
  AmpUI* ui = (AmpUI*)handle;
  return 0;
}

/* Idle interface for optional non-embedded UI. */
static int
ui_idle(LV2UI_Handle handle)
{
  AmpUI* ui = (AmpUI*)handle;
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
