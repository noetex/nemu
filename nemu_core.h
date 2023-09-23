#define NEMU_VERSION 0.1
#define Assert(Condition) if(Condition); else debug_break()
#define return_if(Condition) if(!(Condition)); else return
#define break_if(Condition) if(!(Condition)); else break
#define static_assert(Condition) typedef int static_assert_typedef[(!!(Condition)) ? 1 : -1]
#define array_length(Array) (sizeof(Array)/sizeof(*(Array)))

typedef uint16_t wchar_t;
typedef enum
{
	false,
	true,
	BOOL_FORCE_SIZE = ((int)-1),
} bool_t;

static wchar_t NEMU_WINDOW_TITLE[] = L"nemu";
