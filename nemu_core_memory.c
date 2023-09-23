static int8_t*
translate_address(nes_console* Console, uint16_t Address)
{
	int8_t* Result = 0;
	if(Address >= 0x8000)
	{
		Result = Console->ROM;
		Address &= MEMORY_MASK_ROM;
	}
	else if(Address >= 0x6000)
	{
		Result = Console->SRAM;
		Address &= MEMORY_MASK_SRAM;
	}
	else if(Address >= 0x4020)
	{
		Result = Console->ExROM;
		Address &= MEMORY_MASK_EXROM;
	}
	else if(Address >= 0x4000)
	{
		Result = Console->IO2;
		Address &= MEMORY_MASK_IO2;
	}
	else if(Address >= 0x2000)
	{
		Result = Console->IO1;
		Address &= MEMORY_MASK_IO1;
	}
	else
	{
		Result = Console->RAM;
		Address &= MEMORY_MASK_RAM;
	}
	Result += Address;
	return Result;
}

static int8_t
cpu_memory_read(nes_console* Console, uint16_t Address)
{
	int8_t* Memory = translate_address(Console, Address);
	int8_t Result = *Memory;
	return Result;
}

static void
cpu_memory_write(nes_console* Console, uint16_t Address, uint8_t Value)
{
	int8_t* Memory = translate_address(Console, Address);
	*Memory = Value;
}

static int8_t
cpu_fetch(nes_console* Console)
{
	uint16_t Address = Console->CPU.PC;
	int8_t Result = cpu_memory_read(Console, Address);
	Console->CPU.PC += 1;
	return Result;
}

static uint16_t
get_operand_address(nes_console* Console)
{
	uint16_t Result;
	addressing_mode Mode = ADDRESS_MODE_TABLE[Console->CPU.IR];
	switch(Mode)
	{
		case ADDRESSING_ABSOLUTE:
		{
			uint16_t AddressLo = cpu_fetch(Console);
			uint16_t AddressHi = cpu_fetch(Console);
			Result = (AddressHi << 8) | AddressLo;
		} break;
		case ADDRESSING_ABSOLUTE_X:
		{
			uint16_t BaseLo = cpu_fetch(Console);
			uint16_t BaseHi = cpu_fetch(Console);
			BaseLo += Console->CPU.IndexRegisterX;
			//bool_t PageCrosssed = (BaseLo >> 8);
			Result = (BaseHi << 8) + BaseLo;
		} break;
		case ADDRESSING_ABSOLUTE_Y:
		{
			uint16_t BaseLo = cpu_fetch(Console);
			uint16_t BaseHi = cpu_fetch(Console);
			BaseLo += Console->CPU.IndexRegisterY;
			//bool_t PageCrossed = (BaseLo >> 8);
			Result = (BaseHi << 8) + BaseLo;
		} break;
		case ADDRESSING_ZEROPAGE:
		{
			Result = cpu_fetch(Console);
		} break;
		case ADDRESSING_ZEROPAGE_X:
		{
			uint16_t Base = cpu_fetch(Console);
			uint16_t Offset = Console->CPU.IndexRegisterX;
			Result = (Base + Offset) & 0xff;
		} break;
		case ADDRESSING_ZEROPAGE_Y:
		{
			uint16_t Base = cpu_fetch(Console);
			uint16_t Offset = Console->CPU.IndexRegisterY;
			Result = (Base + Offset) & 0xff;
		} break;
		case ADDRESSING_INDIRECT:
		{
			uint16_t IndirectLo = cpu_fetch(Console);
			uint16_t IndirectHi = cpu_fetch(Console);
			uint16_t Indirect1 = (IndirectHi << 8) | IndirectLo;
			uint16_t Indirect2 = ((Indirect1 + 1) & 0xff) | (Indirect1 & 0xff00);
			uint16_t AddressLo = cpu_memory_read(Console, Indirect1);
			uint16_t AddressHi = cpu_memory_read(Console, Indirect2);
			Result = (AddressHi << 8) | AddressLo;
		} break;
		case ADDRESSING_INDIRECT_X:
		{
			uint16_t Base = cpu_fetch(Console);
			uint16_t Offset = Console->CPU.IndexRegisterX;
			uint16_t Indirect = (Base + Offset) & 0xff;
			uint16_t AddressLo = cpu_memory_read(Console, Indirect);
			uint16_t AddressHi = cpu_memory_read(Console, Indirect + 1);
			Result = (AddressHi << 8) | AddressLo;
		} break;
		case ADDRESSING_INDIRECT_Y:
		{
			uint16_t Indirect = cpu_fetch(Console);
			uint16_t AddressLo = cpu_memory_read(Console, Indirect);
			uint16_t AddressHi = cpu_memory_read(Console, Indirect + 1);
			AddressLo += Console->CPU.IndexRegisterY;
			//bool_t PageCrossed = (AddressLo >> 8);
			Result = (AddressHi << 8) + AddressLo;
		} break;
		default: break;
	}
	return Result;
}

static int8_t
get_operand_value(nes_console* Console)
{
	int8_t Result;
	addressing_mode Mode = ADDRESS_MODE_TABLE[Console->CPU.IR];
	if(Mode == ADDRESSING_IMMEDIATE)
	{
		Result = cpu_fetch(Console);
	}
	else
	{
		uint16_t Address = get_operand_address(Console);
		Result = cpu_memory_read(Console, Address);
	}
	return Result;
}

static int8_t*
get_operand_pointer(nes_console* Console)
{
	int8_t* Result;
	addressing_mode Mode = ADDRESS_MODE_TABLE[Console->CPU.IR];
	if(Mode == ADDRESSING_ACCUMULATOR)
	{
		Result = &Console->CPU.Accumulator;
	}
	else
	{
		uint16_t Address = get_operand_address(Console);
		Result = translate_address(Console, Address);
	}
	return Result;
}
