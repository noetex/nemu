typedef enum
{
	ROM_FORMAT_UNKNOWN,
	ROM_FORMAT_INES,
	ROM_FORMAT_NES2,
	ROM_FORMAT_NESOLD,
	ROM_FORMAT_COUNT,
} rom_format;

typedef enum
{
	ROM_REGION_UNKNOWN,
	ROM_REGION_JP,
	ROM_REGION_NA,
	ROM_REGION_EU,
} rom_region;

typedef struct
{
	uint16_t MapperID;
	bool_t TrainerExists;
	bool_t BatteryExists;
	uint8_t* PrgRomData;
	uint8_t* ChrRomData;
	uint8_t* PrgRamData;
	uint8_t* ChrRamData;
	uint32_t PrgRomSize;
	uint32_t ChrRomSize;
	uint32_t PrgRamSize;
	uint32_t ChrRamSize;
} nes_info;

typedef struct
{
	//char* Path;
	//char* Name;
	rom_format Format;
	//rom_region Region;
	union
	{
		nes_info NesInfo;
	};

	bool_t IsValid;
} rom_info;
