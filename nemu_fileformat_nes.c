static bool_t
is_format_nes(char* FileData)
{
	uint32_t Tag = *(uint32_t*)FileData;
	bool_t Result = Tag == *(uint32_t*)"NES\x1a";
	return Result;
}

static nes_fileformat
nes_get_format(nes_header* Header)
{
	uint32_t Version = Header->Byte7 & 0xc;
	if(Version == 0)
	{
		return NES_FILEFORMAT_I;
	}
	else if(Version == 2)
	{
		return NES_FILEFORMAT_2;
	}
	return NES_FILEFORMAT_OLD;
}

static bool_t
nes_battery_is_present(nes_header* Header)
{
	bool_t Result = (Header->Byte6 >> 1) & 1;
	return Result;
}

static bool_t
nes_trainer_is_present(nes_header* Header)
{
	bool_t Result = (Header->Byte6 >> 2) & 1;
	return Result;
}

static mapper_id
nes_get_mapper_id(nes_header* Header, nes_fileformat Format)
{
	uint16_t MapperNumber0 = (uint16_t)(Header->Byte6 & 0xf0) >> 4;
	uint16_t MapperNumber1 = (uint16_t)(Header->Byte7 & 0xf0);
	uint16_t MapperNumber2 = (uint16_t)(Header->Byte8 & 0x0f) << 8;
	uint16_t Result = MapperNumber0;
	if(Format == NES_FILEFORMAT_I)
	{
		Result |= MapperNumber1;
	}
	else if(Format == NES_FILEFORMAT_2)
	{
		Result |= (MapperNumber1 | MapperNumber2);
	}
	return Result;
}

static uint32_t
nes_get_exponential_size(uint32_t Data)
{
	uint32_t Exponent = Data >> 2;
	uint32_t Multiplier = Data & 0x3;
	uint32_t Power = 1 << (Exponent + 1);
	uint32_t Result = (Multiplier * Power) + (Power >> 1);
	return Result;
}

static uint32_t
nes_get_prgrom_size(nes_header* Header, nes_fileformat Format)
{
	uint32_t NumUnits = Header->NumPrgUnits;
	if(Format == NES_FILEFORMAT_2)
	{
		int UsingExponent = ((Header->Byte9 & 0xf) == 0xf);
		if(UsingExponent)
		{
			uint32_t Result = nes_get_exponential_size(NumUnits);
			return Result;
		}
		NumUnits |= (Header->Byte9 & 0xf << 8);
	}
	else if(NumUnits == 0)
	{
		NumUnits = 256;
	}
	uint32_t Result = NumUnits * PRGROM_UNIT_SIZE;
	return Result;
}

static uint32_t
nes_get_chrrom_size(nes_header* Header, nes_fileformat Format)
{
	uint32_t NumUnits = Header->NumChrUnits;
	if(Format == NES_FILEFORMAT_2)
	{
		int UsingExponent = ((Header->Byte9 >> 4) == 0xf);
		if(UsingExponent)
		{
			uint32_t Result = nes_get_exponential_size(NumUnits);
			return Result;
		}
		NumUnits |= ((Header->Byte9 & 0xf0) << 4);
	}
	uint32_t Result = NumUnits * CHRROM_UNIT_SIZE;
	return Result;
}

static nes_gamesystem
nes_get_gamesystem(nes_header* Header, nes_fileformat Format)
{
	nes_gamesystem Result = NES_GAMESYSTEM_UNKNOWN;
	if(Format == NES_FILEFORMAT_2)
	{
		uint8_t Indicator = Header->Byte7 & 0x3;
		if(Indicator == 3)
		{

		}
	}
	else if(Format == NES_FILEFORMAT_I)
	{

	}
	return Result;
}
