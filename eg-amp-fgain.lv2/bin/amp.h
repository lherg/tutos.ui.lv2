extern const LV2_Descriptor* lv2_descriptor(uint32_t index);
extern const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index);

static const RtkLv2Description _plugin = {
	&lv2_descriptor, &lv2ui_descriptor
	, 0 // uint32_t dsp_descriptor_id
	, 0 // uint32_t gui_descriptor_id
	, "Simple Amplifier"
	, (const struct LV2Port[3])
	{
		{ "gain", CONTROL_IN, 0.f, -12.f, 12.f,"Gain"},		
		{ "in", AUDIO_IN, nan, nan, nan, "Audio Input"},
		{ "out", AUDIO_OUT, nan, nan, nan, "Audio Output"},
	}
	, 3 // uint32_t nports_total
	, 1 // uint32_t nports_audio_in
	, 1 // uint32_t nports_audio_out
	, 0 // uint32_t nports_midi_in
	, 0 // uint32_t nports_midi_out
	, 0 // uint32_t nports_atom_in
	, 0 // uint32_t nports_atom_out
	, 1 // uint32_t nports_ctrl
	, 1 // uint32_t nports_ctrl_in
	, 0 // uint32_t nports_ctrl_out
	, 8192 // uint32_t min_atom_bufsiz
	, false // bool send_time_info
	, UINT32_MAX // uint32_t latency_ctrl_port
};

