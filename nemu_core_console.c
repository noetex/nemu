static void
console_start(nes_console* Console)
{
	
}

static void
console_run(nes_console* Console)
{
	for(;;)
	{
		cpu_execute(Console);
		//ppu_execute(&Console->PPU);
		//apu_execute(&Console->APU);
	}
}
