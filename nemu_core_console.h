typedef struct
{
	cpu_6502 CPU;
	int8_t RAM[0x800];
	int8_t IO1[0x8];
	int8_t IO2[0x20];
	int8_t ExROM[0x1fe0];
	int8_t SRAM[0x2000];
	int8_t ROM[0x8000];
} nes_console;
