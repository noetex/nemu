#define PALLETTE_SIZE 64
#define RGB_COLOR_SIZE 24

typedef enum
{
	MAPPERID_NONE = 0,
	MAPPERID_MMC1,
	MAPPERID_UNROM,
	MAPPERID_CNROM,
	MAPPERID_MMC3,
	MAPPERID_MMC5,
	MAPPERID_FFE_F4,
	MAPPERID_AOROM,
	MAPPERID_FFE_F3,
	MAPPERID_MMC2,
	MAPPERID_MMC4,
	MAPPERID_COLORDREAMS,
	MAPPERID_FFE_F6,
	MAPPERID_100_IN_1 = 15,
	MAPPERID_BANDAI,
	MAPPERID_FFE_F8,
	MAPPERID_JALECO_SS8806,
	MAPPERID_NAMCOT_106,
	MAPPERID_DISKSYSTEM,
	MAPPERID_KONAMI_VRC4A,
	MAPPERID_KONAMI_VRC2A,
	MAPPERID_KONAMI_VRC2A_2,
	MAPPERID_KONAMI_VRC6,
	MAPPERID_KONAMI_VRC4B,
	MAPPERID_IREM_G101 = 32,
	MAPPERID_TAITO_TC0,
	MAPPERID_32KB_ROM,
	MAPPERID_TENGEN_RAMBO1 = 64,
	MAPPERID_IREM_H3001,
	MAPPERID_GNROM,
	MAPPERID_SUNSOFT3,
	MAPPERID_SUNSOFT4,
	MAPPERID_SUNSOFT5,
	MAPPERID_CAMERICA = 71,
	MAPPERID_IREM_74HC161 = 78,
	MAPPERID_PIRATE_HK_SF3 = 91,
} mapper_id;
