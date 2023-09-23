DEFINE_INSTRUCTION_6502(ADC)
{
	int16_t Operand = get_operand_value(Console);
	int16_t Carry = Console->CPU.Status.Carry;
	int16_t Accumulator = Console->CPU.Accumulator;
	int16_t Result = Accumulator + Operand + Carry;
	Console->CPU.Status.Carry = (Result >> 8);
	Console->CPU.Status.Overflow = (Result > 127) | (Result < -128);
	Console->CPU.Status.Negative = (Result >> 7);
	Console->CPU.Status.Zero = (Result == 0);
	Console->CPU.Accumulator = (int8_t)Result;
}

DEFINE_INSTRUCTION_6502(AND)
{
	int8_t Operand = get_operand_value(Console);
	int8_t Accumulator = Console->CPU.Accumulator;
	int8_t Result = Accumulator & Operand;
	Console->CPU.Status.Zero = (Result == 0);
	Console->CPU.Status.Negative = (Result >> 7);
	Console->CPU.Accumulator = Result;
}

DEFINE_INSTRUCTION_6502(ASL)
{
	int8_t* Destination = get_operand_pointer(Console);
	int8_t Value = *Destination;
	int8_t Result = Value << 1;
	Console->CPU.Status.Zero = (Result == 0);
	Console->CPU.Status.Carry = (Value >> 7);
	*Destination = Result;
}

DEFINE_INSTRUCTION_6502(BCC)
{
	bool_t Condition = !Console->CPU.Status.Carry;
	cpu_branch(Console, Condition);
}

DEFINE_INSTRUCTION_6502(BCS)
{
	bool_t Condition = Console->CPU.Status.Carry;
	cpu_branch(Console, Condition);
}

DEFINE_INSTRUCTION_6502(BEQ)
{
	bool_t Condition = Console->CPU.Status.Zero;
	cpu_branch(Console, Condition);
}

DEFINE_INSTRUCTION_6502(BIT)
{
	int8_t Operand = get_operand_value(Console);
	int8_t Accumulator = Console->CPU.Accumulator;
	int8_t Result = Accumulator & Operand;
	Console->CPU.Status.Negative = (Result >> 7) & 1;
	Console->CPU.Status.Overflow = (Result >> 6) & 1;
	Console->CPU.Status.Zero = (Result == 0);
}

DEFINE_INSTRUCTION_6502(BMI)
{
	bool_t Condition = Console->CPU.Status.Negative;
	cpu_branch(Console, Condition);
}

DEFINE_INSTRUCTION_6502(BNE)
{
	bool_t Condition = !Console->CPU.Status.Zero;
	cpu_branch(Console, Condition);
}

DEFINE_INSTRUCTION_6502(BPL)
{
	bool_t Condition = !Console->CPU.Status.Negative;
	cpu_branch(Console, Condition);
}

DEFINE_INSTRUCTION_6502(BRK)
{
	cpu_interrupt(Console, INTERRUPT_TYPE_BRK);
}

DEFINE_INSTRUCTION_6502(BVC)
{
	bool_t Condition = !Console->CPU.Status.Overflow;
	cpu_branch(Console, Condition);
}

DEFINE_INSTRUCTION_6502(BVS)
{
	bool_t Condition = Console->CPU.Status.Overflow;
	cpu_branch(Console, Condition);
}

DEFINE_INSTRUCTION_6502(CLC)
{
	Console->CPU.Status.Carry = 0;
}

DEFINE_INSTRUCTION_6502(CLD)
{
	Console->CPU.Status.DecimalMode = 0;
}

DEFINE_INSTRUCTION_6502(CLI)
{
	Console->CPU.Status.InterruptDisable = 0;
}

DEFINE_INSTRUCTION_6502(CLV)
{
	Console->CPU.Status.Overflow = 0;
}

DEFINE_INSTRUCTION_6502(CMP)
{
	int8_t Register = Console->CPU.Accumulator;
	int8_t Operand = get_operand_value(Console);
	cpu_compare(&Console->CPU, Register, Operand);
}

DEFINE_INSTRUCTION_6502(CPX)
{
	int8_t Register = Console->CPU.IndexRegisterX;
	int8_t Operand = get_operand_value(Console);
	cpu_compare(&Console->CPU, Register, Operand);
}

DEFINE_INSTRUCTION_6502(CPY)
{
	int8_t Register = Console->CPU.IndexRegisterY;
	int8_t Operand = get_operand_value(Console);
	cpu_compare(&Console->CPU, Register, Operand);
}

DEFINE_INSTRUCTION_6502(DEC)
{
	int8_t* Destination = get_operand_pointer(Console);
	cpu_decrement(&Console->CPU, Destination);
}

DEFINE_INSTRUCTION_6502(DEX)
{
	int8_t* Destination = &Console->CPU.IndexRegisterX;
	cpu_decrement(&Console->CPU, Destination);
}

DEFINE_INSTRUCTION_6502(DEY)
{
	int8_t* Destination = &Console->CPU.IndexRegisterY;
	cpu_decrement(&Console->CPU, Destination);
}

DEFINE_INSTRUCTION_6502(EOR)
{
	int8_t Operand = get_operand_value(Console);
	int8_t Accumulator = Console->CPU.Accumulator;
	int8_t Result = Accumulator ^ Operand;
	Console->CPU.Status.Zero = (Result == 0);
	Console->CPU.Status.Negative = (Result < 0);
	Console->CPU.Accumulator = Result;
}

DEFINE_INSTRUCTION_6502(INC)
{
	int8_t* Destination = get_operand_pointer(Console);
	cpu_increment(&Console->CPU, Destination);
}

DEFINE_INSTRUCTION_6502(INX)
{
	int8_t* Destination = &Console->CPU.IndexRegisterX;
	cpu_increment(&Console->CPU, Destination);
}

DEFINE_INSTRUCTION_6502(INY)
{
	int8_t* Destination = &Console->CPU.IndexRegisterY;
	cpu_increment(&Console->CPU, Destination);
}

DEFINE_INSTRUCTION_6502(JMP)
{
	uint16_t Address = get_operand_address(Console);
	Console->CPU.PC = Address;
}

DEFINE_INSTRUCTION_6502(JSR)
{
	uint16_t Address = get_operand_address(Console);
	Console->CPU.PC = Address;
}

DEFINE_INSTRUCTION_6502(LDA)
{
	int8_t* Destination = &Console->CPU.Accumulator;
	cpu_load(Console, Destination);
}

DEFINE_INSTRUCTION_6502(LDX)
{
	int8_t* Destination = &Console->CPU.IndexRegisterX;
	cpu_load(Console, Destination);
}

DEFINE_INSTRUCTION_6502(LDY)
{
	int8_t* Destination = &Console->CPU.IndexRegisterY;
	cpu_load(Console, Destination);
}

DEFINE_INSTRUCTION_6502(LSR)
{
	int8_t* Destination = get_operand_pointer(Console);
	uint8_t Value = *Destination;
	uint8_t Result = Value >> 1;
	Console->CPU.Status.Carry = Value & 1;
	Console->CPU.Status.Negative = 0;
	Console->CPU.Status.Zero = (Result == 0);
	*Destination = Result;
}

DEFINE_INSTRUCTION_6502(NOP)
{
	(void)get_operand_address(Console);
}

DEFINE_INSTRUCTION_6502(ORA)
{
	int8_t Operand = get_operand_value(Console);
	int8_t Accumulator = Console->CPU.Accumulator;
	int8_t Result = Accumulator | Operand;
	Console->CPU.Status.Zero = (Result == 0);
	Console->CPU.Status.Negative = (Result >> 7);
	Console->CPU.Accumulator = Result;
}

DEFINE_INSTRUCTION_6502(PHA)
{
	int8_t Value = Console->CPU.Accumulator;
	cpu_push(Console, Value);
}

DEFINE_INSTRUCTION_6502(PHP)
{
	int8_t Value = Console->CPU.Status.Value;
	cpu_push(Console, Value);
}

DEFINE_INSTRUCTION_6502(PLA)
{
	int8_t* Destination = &Console->CPU.Accumulator;
	cpu_pop(Console, Destination);
}

DEFINE_INSTRUCTION_6502(PLP)
{
	int8_t* Destination = &Console->CPU.Status.Value;
	cpu_pop(Console, Destination);
}

DEFINE_INSTRUCTION_6502(ROL)
{
	int8_t* Destination = get_operand_pointer(Console);
	int8_t Value = *Destination;
	int8_t Carry = Console->CPU.Status.Carry;
	int8_t Result = (Value << 1) | Carry;
	Console->CPU.Status.Carry = (Value >> 7);
	Console->CPU.Status.Negative = (Result >> 7);
	Console->CPU.Status.Zero = (Result == 0);
	*Destination = Result;
}

DEFINE_INSTRUCTION_6502(ROR)
{
	int8_t* Destination = get_operand_pointer(Console);
	int8_t Value = *Destination;
	int8_t Carry = Console->CPU.Status.Carry;
	int8_t Result = (Value >> 1) | (Carry << 7);
	Console->CPU.Status.Negative = Carry;
	Console->CPU.Status.Zero = (Result == 0);
	Console->CPU.Status.Carry = (Value & 1);
	*Destination = Result;
}

DEFINE_INSTRUCTION_6502(RTI)
{
	int8_t* Status = &Console->CPU.Status.Value;
	int8_t* LoPC = &(((uint8_t*)&Console->CPU.PC)[0]);
	int8_t* HiPC = &(((uint8_t*)&Console->CPU.PC)[1]);
	cpu_pop(Console, Status);
	cpu_pop(Console, LoPC);
	cpu_pop(Console, HiPC);
}

DEFINE_INSTRUCTION_6502(RTS)
{
	int8_t* LoPC = &(((uint8_t*)&Console->CPU.PC)[0]);
	int8_t* HiPC = &(((uint8_t*)&Console->CPU.PC)[1]);
	cpu_pop(Console, LoPC);
	cpu_pop(Console, HiPC);
}

DEFINE_INSTRUCTION_6502(SBC)
{
	int8_t Operand = get_operand_value(Console);
	int16_t Borrow = !Console->CPU.Status.Carry;;
	int16_t Accumulator = Console->CPU.Accumulator;
	int16_t Result = Accumulator - Operand - Borrow;
	Console->CPU.Status.Carry = (Result >= 0);
	Console->CPU.Status.Overflow = (Result > 127) | (Result < -128);
	Console->CPU.Status.Negative = (Result < 0);
	Console->CPU.Status.Zero = (Result == 0);
	Console->CPU.Accumulator = (int8_t)Result;
}

DEFINE_INSTRUCTION_6502(SEC)
{
	Console->CPU.Status.Carry = 1;
}

DEFINE_INSTRUCTION_6502(SED)
{
	Console->CPU.Status.DecimalMode = 1;
}

DEFINE_INSTRUCTION_6502(SEI)
{
	Console->CPU.Status.InterruptDisable = 1;
}

DEFINE_INSTRUCTION_6502(STA)
{
	int8_t Value = Console->CPU.Accumulator;
	cpu_store(Console, Value);
}

DEFINE_INSTRUCTION_6502(STX)
{
	int8_t Value = Console->CPU.IndexRegisterX;
	cpu_store(Console, Value);
}

DEFINE_INSTRUCTION_6502(STY)
{
	int8_t Value = Console->CPU.IndexRegisterY;
	cpu_store(Console, Value);
}

DEFINE_INSTRUCTION_6502(TAS)
{
	int8_t Value = Console->CPU.Accumulator;
	int8_t* Destination = &Console->CPU.StackPointer;
	cpu_transfer(Console, Destination, Value);
}

DEFINE_INSTRUCTION_6502(TAX)
{
	int8_t Value = Console->CPU.Accumulator;
	int8_t* Destination = &Console->CPU.IndexRegisterX;
	cpu_transfer(Console, Destination, Value);
}

DEFINE_INSTRUCTION_6502(TAY)
{
	int8_t Value = Console->CPU.Accumulator;
	int8_t* Destination = &Console->CPU.IndexRegisterY;
	cpu_transfer(Console, Destination, Value);
}

DEFINE_INSTRUCTION_6502(TSX)
{
	int8_t Value = Console->CPU.StackPointer;
	int8_t* Destination = &Console->CPU.IndexRegisterX;
	cpu_transfer(Console, Destination, Value);
}

DEFINE_INSTRUCTION_6502(TXA)
{
	int8_t Value = Console->CPU.IndexRegisterX;
	int8_t* Destination = &Console->CPU.Accumulator;
	cpu_transfer(Console, Destination, Value);
}

DEFINE_INSTRUCTION_6502(TXS)
{
	int8_t Value = Console->CPU.IndexRegisterX;
	int8_t* Destination = &Console->CPU.StackPointer;
	cpu_transfer(Console, Destination, Value);
}

DEFINE_INSTRUCTION_6502(TYA)
{
	int8_t Value = Console->CPU.IndexRegisterY;
	int8_t* Destination = &Console->CPU.Accumulator;
	cpu_transfer(Console, Destination, Value);
}

DEFINE_INSTRUCTION_6502(XXX)
{
	debug_break();
}
