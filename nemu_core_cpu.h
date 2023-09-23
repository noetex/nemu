#define STACK_BASE 0x100

enum memory_masks
{
	MEMORY_MASK_RAM = 0x3ff,
	MEMORY_MASK_ROM = 0x7ff,
	MEMORY_MASK_IO1 = 0x7,
	MEMORY_MASK_IO2 = 0x3ff,
	MEMORY_MASK_SRAM = 0x3ff,
	MEMORY_MASK_EXROM = 0x3ff,
};

typedef enum
{
	INTERRUPT_TYPE_NMI = 0,
	INTERRUPT_TYPE_RESET = 0x2,
	INTERRUPT_TYPE_IRQ = 0x4,
	INTERRUPT_TYPE_BRK = INTERRUPT_TYPE_IRQ,
} interrupt_type;

typedef union
{
	uint8_t Value;
	struct
	{
		uint8_t Carry : 1;
		uint8_t Zero : 1;
		uint8_t InterruptDisable : 1;
		uint8_t DecimalMode : 1;
		uint8_t Break : 1;
		uint8_t Reserved : 1;
		uint8_t Overflow : 1;
		uint8_t Negative : 1;
	};
} cpu_status;

static_assert(sizeof(cpu_status) == sizeof(uint8_t));

typedef struct
{
	uint16_t PC;
	int8_t IR;
	int8_t Accumulator;
	int8_t IndexRegisterX;
	int8_t IndexRegisterY;
	int8_t StackPointer;
	cpu_status Status;
} cpu_6502;

static_assert(sizeof(cpu_6502) == sizeof(uint64_t));
