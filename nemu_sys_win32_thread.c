static LRESULT CALLBACK
dialog_proc(HWND Dialog, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
	switch(Message)
	{
		case WM_INITDIALOG:
		{

		} break;
		case WM_SETFONT:
		{

		} break;
		case WM_CLOSE:
		{
			EndDialog(Dialog, 0);
		} break;
		case WM_COMMAND:
		{
			DWORD ID = LOWORD(wParam);
			if(ID == IDOK || ID == IDCANCEL)
			{
				EndDialog(Dialog, 0);
			}
		} break;
		case WM_KEYDOWN:
		{
			DWORD VirtualKey = LOWORD(wParam);
			if(VirtualKey == VK_ESCAPE)
			{
				DestroyWindow(Dialog);
			}
		} break;
	}
	return Result;
}

static void
query_display_devices(void)
{
	DISPLAY_DEVICEW Device = {0};
	Device.cb = sizeof(Device);
	DWORD NumDevices = 0;
	while(EnumDisplayDevicesW(0, NumDevices, &Device, 0))
	{
		NumDevices += 1;
		if(Device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
		{
			// This device is usable, record it
		}
	}
}

static WCHAR*
openfilename_dialog(HWND Window, WCHAR* DialogFilter)
{
	static WCHAR FileName[256] = {0};
	OPENFILENAMEW OpenFileName = {0};
	OpenFileName.lStructSize = sizeof(OpenFileName);
	OpenFileName.hwndOwner = Window;
	OpenFileName.lpstrFile = FileName;
	OpenFileName.nMaxFile = array_length(FileName);
	OpenFileName.lpstrFilter = DialogFilter;
	OpenFileName.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	GetOpenFileNameW(&OpenFileName);
	WCHAR* Result = FileName;
	return Result;
}

static void
handle_menu_message(nes_emulator* Emulator, DWORD MenuItem)
{
	HWND Window = Emulator->Window;
	switch(MenuItem)
	{
		case MENUITEM_GAME_LOAD:
		{
			static WCHAR DialogFilter[] =
			{
				L"NES Binary File (.nes)\0*.nes\0"
				L"MOS 6502 Binary (.bin)\0*.bin\0"
				L"All Files\0*.*\0\0"
			};
			WCHAR* FileName = openfilename_dialog(Window, DialogFilter);
			return_if(FileName[0] == '\0');
			HANDLE File = CreateFileW(FileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			Assert(File != INVALID_HANDLE_VALUE);
			DWORD FileSize = GetFileSize(File, 0);
			if(Emulator->GameData)
			{
				VirtualFree(Emulator->GameData, 0, MEM_RELEASE);
			}
			Emulator->GameData = VirtualAlloc(0, FileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			DWORD BytesRead;
			ReadFile(File, Emulator->GameData, FileSize, &BytesRead, 0);
			Assert(BytesRead == FileSize);
			CloseHandle(File);
			emulator_start(Emulator);
		} break;
		case MENUITEM_GAME_SHUTDOWN:
		{
			VirtualFree(Emulator->GameData, 0, MEM_RELEASE);
			Emulator->GameData = 0;
		} break;
		case MENUITEM_CONFIG_VIDEO:
		{
			//WCHAR* TemplateName = MAKEINTRESOURCEW(DIALOGID_CONFIG_VIDEO);
			//DialogBoxParamW(0, TemplateName, Window, dialog_proc, 0);	
		} break;
		case MENUITEM_DEBUG_LOGCONSOLE:
		{
		} break;
		case MENUITEM_MISC_ABOUT:
		{
			WCHAR* TemplateName = MAKEINTRESOURCEW(DIALOGID_MISC_ABOUT);
			DialogBoxParamW(0, TemplateName, Window, dialog_proc, 0);
		} break;
	}
}

static bool_t
process_messages(nes_emulator* Emulator)
{
	MSG Message;
	bool_t Result = false;
	HWND Window = Emulator->Window;
	while(PeekMessageW(&Message, 0, 0, 0, PM_REMOVE))
	{
		switch(Message.message)
		{
			case WM_COMMAND:
			{
				DWORD MenuItem = LOWORD(Message.wParam);
				handle_menu_message(Emulator, MenuItem);
			} break;
			case WM_KEYDOWN:
			{
				DWORD VirtualKey = LOWORD(Message.wParam);
				if(VirtualKey == VK_ESCAPE)
				{
					emulator_pause(Emulator);
				}
			} break;
			case WM_USER_IDLE:
			{
				WaitMessage();
			} break;
			case WM_CLOSE:
			case WM_DESTROY:
			{
				Result = true;
			} break;
		}
	}
	return Result;
}

static DWORD WINAPI
emulator_thread(LPVOID Parameter)
{
	HWND Window = Parameter;
	//HANDLE Settings = CreateFileW(L"settings.ini", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	nes_emulator* Emulator = VirtualAlloc(0, sizeof(*Emulator), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	Emulator->Window = Window;
	//Emulator->Settings = Settings;

	for(;;)
	{
		bool_t ShouldQuit = process_messages(Emulator);
		if(ShouldQuit)
		{
			goto label_thread_exit;
		}
		PostMessageW(Window, WM_USER_IDLE, 0, 0);
	}
label_thread_exit:
	PostMessageW(Window, WM_QUIT, 0, 0);
	return 0;
}
