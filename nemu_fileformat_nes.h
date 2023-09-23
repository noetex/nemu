#define FILE_FORMAT_TAG_NES (*(uint32_t*)"NES\x1a")
#define PRGROM_UNIT_SIZE (16 << 10)
#define CHRROM_UNIT_SIZE (8 << 10)

typedef enum
{
	NES_FILEFORMAT_OLD,
	NES_FILEFORMAT_I,
	NES_FILEFORMAT_2,
} nes_fileformat;

typedef enum
{
	NES_MIRRORING_HORIZONTAL = 0,
	NES_MIRRORING_VERTICAL,
} nes_mirroring;

typedef enum
{
	NES_TVSYSTEM_NTSC = 0,
	NES_TVSYSTEM_PAL,
} nes_tvsystem;

typedef enum
{
	NES_GAMESYSTEM_UNKNOWN = -1,
	NES_GAMESYSTEM_NTSC,
	NES_GAMESYSTEM_PAL,
	NES_GAMESYSTEM_DUAL,
	NES_GAMESYSTEM_VS,
	NES_GAMESYSTEM_PLAYCHOICE,
} nes_gamesystem;

typedef struct
{
	uint32_t Tag;
	uint8_t NumPrgUnits;
	uint8_t NumChrUnits;
	uint8_t Byte6;
	uint8_t Byte7;
	uint8_t Byte8;
	uint8_t Byte9;
	uint8_t Byte10;
	uint8_t Byte11;
	uint8_t Byte12;
	uint8_t Byte13;
	uint8_t Byte14;
	uint8_t Byte15;
} nes_header;

static_assert(sizeof(nes_header) == 16);

typedef struct
{
	uint8_t Data[512];
} nes_trainer;

static_assert(sizeof(nes_trainer) == 512);
