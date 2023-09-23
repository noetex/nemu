static rom_info
read_rom_info(uint8_t* FileData, size_t FileSize, uint8_t** PRGROM)
{
	rom_info Result = {0};
	uint32_t Tag = *(uint32_t*)FileData;
	if(Tag == *(uint32_t*)"NES\x1a")
	{
		nes_header* Header = (nes_header*)FileData;
		Result.Format = nes_get_format(Header);
		Result.NesInfo.MapperID = nes_get_mapper_id(Header, Result.Format);
		Result.NesInfo.TrainerExists = nes_trainer_is_present(Header);
		Result.NesInfo.BatteryExists = nes_battery_is_present(Header);
		Result.NesInfo.PrgRomSize = nes_get_prgrom_size(Header, Result.Format);
		Result.NesInfo.ChrRomSize = nes_get_chrrom_size(Header, Result.Format);
		FileData += sizeof(*Header);
		if(Result.NesInfo.TrainerExists)
		{
			nes_trainer* Trainer = (nes_trainer*)FileData;
			(void)Trainer;
			FileData += sizeof(*Trainer);
		}
		*PRGROM = FileData;
	}
	return Result;
}

static void
print_rom_info(rom_info* RomInfo)
{
	char* Unknown = "Unknown";
	char* YesOrNo[3] = {"No", "Yes", "Unknown"};
	char* PresentOrNot[3] = {"Not Exists", "Exists", Unknown};
	char* Info =
	{
		"Name: %s\n"
		"Path: %s\n"
		"Format: %s\n"
		"Region: %s\n"
		"PRGROM Size: %s\n"
		"CHRROM Size: %s\n"
		"PRGRAM Size: %s\n"
		"CHRRAM Size: %s\n"
		"Persistent Memory: %s\n"
		"Mapper ID: %s\n"
		"Trainer: %s\n"
	};
}
