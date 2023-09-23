#if 0
static emulator_settings
load_settings(wchar_t* FileName)
{
	
}

static void
emulator_init(emulator_state* Emulator)
{
	Emulator->Settings = load_settings(L"settings.cfg");
}


static emulator_settings
load_settings(void)
{
	file_handle File = file_open("settings.cfg");
	emulator_settings Result = 
}



static emulator_state*
emulator_create(window_handle Window)
{
	emulator_state* Result = malloc(sizeof(*Result));
	Result.Settings = load_settings();
	return Result;
}
#endif

static void
emulator_pause(nes_emulator* Emulator)
{
	
}

static void
emulator_start(nes_emulator* Emulator)
{

}
