static void
cpu_branch(nes_console* Console, bool_t Condition)
{
	int8_t Offset = cpu_fetch(Console);
	if(Condition)
	{
		Console->CPU.PC += Offset;
	}
}

static void
cpu_compare(cpu_6502* CPU, int8_t Register, int8_t Operand)
{
	int8_t Result = Register - Operand;
	CPU->Status.Zero = (Result == 0);
	CPU->Status.Negative = (Result >> 7) & 1;
	CPU->Status.Carry = (Result <= (uint8_t)Register);
}

static void
cpu_decrement(cpu_6502* CPU, int8_t* Destination)
{
	int8_t Value = *Destination;
	int8_t Result = Value - 1;
	CPU->Status.Zero = (Result == 0);
	CPU->Status.Negative = (Result >> 7);
	*Destination = Result;
}

static void
cpu_increment(cpu_6502* CPU, int8_t* Destination)
{
	int8_t Value = *Destination;
	int8_t Result = Value + 1;
	CPU->Status.Zero = (Result == 0);
	CPU->Status.Negative = (Result >> 7);
	*Destination = Result;
}

static void
cpu_load(nes_console* Console, int8_t* Destination)
{
	int8_t Result = get_operand_value(Console);
	Console->CPU.Status.Zero = (Result == 0);
	Console->CPU.Status.Negative = (Result >> 7);
	*Destination = Result;
}

static void
cpu_store(nes_console* Console, int8_t Value)
{
	uint16_t Address = get_operand_address(Console);
	cpu_memory_write(Console, Address, Value);
}

static void
cpu_transfer(nes_console* Console, int8_t* Destination, int8_t Value)
{
	*Destination = Value;
	Console->CPU.Status.Negative = (Value < 0);
	Console->CPU.Status.Zero = (Value == 0);
}

static void
cpu_push(nes_console* Console, int8_t Value)
{
	uint8_t Offset = Console->CPU.StackPointer;
	uint16_t Address = STACK_BASE + Offset;
	cpu_memory_write(Console, Address, Value);
	Console->CPU.StackPointer -= 1;
}

static void
cpu_pop(nes_console* Console, int8_t* Destination)
{
	Console->CPU.StackPointer += 1;
	uint8_t Offset = Console->CPU.StackPointer;
	uint16_t Address = STACK_BASE + Offset;
	int8_t Result = cpu_memory_read(Console, Address);
	Console->CPU.Status.Negative = (Result >> 7);
	Console->CPU.Status.Zero = (Result == 0);
	*Destination = Result;
}

static void
cpu_interrupt(nes_console* Console, interrupt_type Type)
{
	cpu_6502* CPU = &Console->CPU;
	cpu_push(Console, (uint8_t)(CPU->PC));
	cpu_push(Console, (uint8_t)(CPU->PC >> 8));
	cpu_push(Console, CPU->Status.Value);
	uint16_t LoPC = cpu_memory_read(Console, 0xfffa + Type);
	uint16_t HiPC = cpu_memory_read(Console, 0xfffb + Type);
	CPU->PC = (HiPC << 8) | LoPC;
	if(Type == INTERRUPT_TYPE_NMI)
	{
		//cpu_push(CPU);

	}
}

static void
cpu_reset(nes_console* Console)
{
	Console->CPU = (cpu_6502){0};
	uint16_t LoPC = cpu_memory_read(Console, 0xfffc);
	uint16_t HiPC = cpu_memory_read(Console, 0xfffd);
	Console->CPU.PC = (HiPC << 8) | LoPC;
}

static void
cpu_execute(nes_console* Console)
{
	for(;;)
	{
		Console->CPU.IR = cpu_fetch(Console);
		Console->CPU.PC += 1;
		OPCODE_TABLE[Console->CPU.IR](Console);
		//Cycles -= 1;
		//break_if(CPU->Stop);
	}
}
