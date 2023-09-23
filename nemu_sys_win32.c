#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<commdlg.h>
#include<stdint.h>

#include"nemu_sys_win32.h"
#include"nemu_sys_win32_resource.h"
#include"nemu_core.h"
#include"nemu_core_cpu.h"
#include"nemu_core_mappers.h"
#include"nemu_core_console.h"
#include"nemu_core_opcodes.h"
#include"nemu_core_emulator.h"
#include"nemu_fileformat_nes.h"

#include"nemu_fileformat_nes.c"
#include"nemu_core.c"
#include"nemu_core_memory.c"
#include"nemu_core_cpu.c"
#include"nemu_core_opcodes.c"
#include"nemu_core_console.c"
#include"nemu_core_emulator.c"
#include"nemu_sys_win32_thread.c"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "comdlg32.lib")

static DWORD EmulatorThreadID;
static WCHAR* NEMU_WNDCLASS_NAME = L"nemu_wndclass";
static WCHAR* RESOURCE_NAME_MAINMENU = MAKEINTRESOURCEW(CONTROLID_MAINMENU);

static LRESULT CALLBACK
window_proc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
	switch(Message)
	{
		case WM_CREATE:
		{
			HWND ImageSurface = CreateWindowExW(0, L"Static", 0, WS_CHILD | WS_VISIBLE | SS_BITMAP, 0, 0, 512, 480, Window, 0, 0, 0);
			HBITMAP Bitmap = LoadImageW(0, L"../nemu_image.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			SendMessageW(ImageSurface, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)Bitmap);
		} break;
		case WM_PAINT:
		{
			PAINTSTRUCT PaintStruct;
			HDC DeviceContext = BeginPaint(Window, &PaintStruct);
			(void)DeviceContext;
			EndPaint(Window, &PaintStruct);
		} break;
		//case WM_SIZE:
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_KEYDOWN:
		case WM_COMMAND:
		{
			PostThreadMessageW(EmulatorThreadID, Message, wParam, lParam);
		} break;
		default:
		{
			Result = DefWindowProcW(Window, Message, wParam, lParam);
		} break;
	}
	return Result;
}

static HMENU
create_window_menu(void)
{
	HMENU Game = CreatePopupMenu();
	AppendMenuW(Game, MF_STRING, MENUITEM_GAME_LOAD, L"Load...");
	AppendMenuW(Game, MF_SEPARATOR, 0, 0);
	AppendMenuW(Game, MF_STRING | MF_GRAYED, MENUITEM_GAME_RESUME, L"Resume");
	AppendMenuW(Game, MF_SEPARATOR, 0, 0);
	AppendMenuW(Game, MF_STRING | MF_GRAYED, MENUITEM_GAME_LOADSTATE, L"Load State");
	AppendMenuW(Game, MF_STRING | MF_GRAYED, MENUITEM_GAME_SAVESTATE, L"Save State");
	HMENU Config = CreatePopupMenu();
	AppendMenuW(Config, MF_STRING, MENUITEM_CONFIG_EMULATION, L"Emulation");
	AppendMenuW(Config, MF_STRING, MENUITEM_CONFIG_VIDEO, L"Video");
	AppendMenuW(Config, MF_STRING, MENUITEM_CONFIG_AUDIO, L"Audio");
	AppendMenuW(Config, MF_STRING, MENUITEM_CONFIG_INPUT, L"Input");
	HMENU Debug = CreatePopupMenu();
	AppendMenuW(Debug, MF_STRING, MENUITEM_DEBUG_DEBUGGER, L"Debugger");
	AppendMenuW(Debug, MF_STRING, MENUITEM_DEBUG_DISASSEMBLY, L"Disassembly");
	AppendMenuW(Debug, MF_STRING, MENUITEM_DEBUG_LOGCONSOLE, L"Log Console");
	HMENU Misc = CreatePopupMenu();
	AppendMenuW(Misc, MF_STRING, MENUITEM_MISC_FAQ, L"FAQ");
	AppendMenuW(Misc, MF_STRING, MENUITEM_MISC_ABOUT, L"About");
	HMENU Result = CreateMenu();
	AppendMenuW(Result, MF_STRING | MF_POPUP, (UINT_PTR)Game, L"Game");
	AppendMenuW(Result, MF_STRING | MF_POPUP, (UINT_PTR)Config, L"Config");
	AppendMenuW(Result, MF_STRING | MF_POPUP, (UINT_PTR)Debug, L"Debug");
	AppendMenuW(Result, MF_STRING | MF_POPUP, (UINT_PTR)Misc, L"Misc");
	return Result;
}

static HWND
create_main_window(void)
{
	WNDCLASSEXW WindowClass = {0};
	WindowClass.cbSize = sizeof(WindowClass);
	WindowClass.lpszClassName = NEMU_WNDCLASS_NAME;
	WindowClass.lpfnWndProc = window_proc;
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassExW(&WindowClass);
	RECT Border = {0, 0, 512, 480};
	DWORD Style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD StyleEx = WS_EX_WINDOWEDGE;
	AdjustWindowRectEx(&Border, Style, TRUE, StyleEx);
	uint32_t Width = Border.right - Border.left;
	uint32_t Height = Border.bottom - Border.top;
	HMENU Menu = LoadMenuW(0, RESOURCE_NAME_MAINMENU);
	HWND Result = CreateWindowExW(StyleEx, NEMU_WNDCLASS_NAME, NEMU_WINDOW_TITLE, Style, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, 0, Menu, 0, 0);
	SetForegroundWindow(Result);
	return Result;
}

static void
disable_dpi_scaling(void)
{
	HMODULE Library = LoadLibraryW(L"user32.dll");
	void* Function = GetProcAddress(Library, "SetProcessDpiAwarenessContext");
	if(Function)
	{
		typedef BOOL dpi_new_function(DPI_AWARENESS_CONTEXT);
		dpi_new_function* SetProcessDpiAwarenessContext = Function;
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		return;
	}
	Function = GetProcAddress(Library, "SetProcessDpiAware");
	if(Function)
	{
		typedef BOOL dpi_old_function(void);
		dpi_old_function* SetProcessDpiAware = Function;
		SetProcessDpiAware();
	}
}

void WinMainCRTStartup(void)
{
	disable_dpi_scaling();
	HWND Window = FindWindowW(NEMU_WNDCLASS_NAME, NEMU_WINDOW_TITLE);
	if(Window)
	{
		WCHAR* Message = L"NEMU is already running. Launch another instance?";
		UINT MessageType = MB_YESNO | MB_ICONINFORMATION;
		int Response = MessageBoxW(Window, Message, NEMU_WINDOW_TITLE, MessageType);
		if(Response == IDNO)
		{
			goto label_program_exit;
		}
	}

	Window = create_main_window();
	CreateThread(0, 0, emulator_thread, Window, 0, &EmulatorThreadID);

	for(;;)
	{
		MSG Message;
		GetMessageW(&Message, 0, 0, 0);
		break_if(Message.message == WM_QUIT);
		DispatchMessageW(&Message);
	}
label_program_exit:
	ExitProcess(0);
}
