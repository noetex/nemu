typedef struct
{
	size_t Version;
	size_t UsedVideoRenderer;
	size_t UsedAudioRenderer;

	//intptr_t* AvailableVideoCards;
	//intptr_t* AvailableAudioCards;
	//void* AvailableVideoCards;
} emulator_settings;

typedef struct
{
	window_handle Window;
	//file_handle Settings;
	nes_console Console;
	//rom_info CurrentROM;
	uint8_t* GameData;
	emulator_settings Settings;
} nes_emulator;

//static emulator_settings EMULATOR_SETTINGS_DEFAULT = {0};
