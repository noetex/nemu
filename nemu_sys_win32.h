#define debug_break() __debugbreak()

typedef HWND window_handle;

enum window_menu_items
{
	MENUITEM_GAME_LOAD = 0x8000,
	MENUITEM_GAME_RESUME,
	MENUITEM_GAME_SAVESTATE,
	MENUITEM_GAME_LOADSTATE,
	MENUITEM_GAME_SHUTDOWN,
	MENUITEM_CONFIG_EMULATION,
	MENUITEM_CONFIG_VIDEO,
	MENUITEM_CONFIG_AUDIO,
	MENUITEM_CONFIG_INPUT,
	MENUITEM_DEBUG_DEBUGGER,
	MENUITEM_DEBUG_LOGCONSOLE,
	MENUITEM_DEBUG_DISASSEMBLY,
	MENUITEM_MISC_FAQ,
	MENUITEM_MISC_CHECKUPDATE,
	MENUITEM_MISC_ABOUT,
};

enum custom_window_messages
{
	WM_USER_IDLE = WM_USER,
};

static BYTE __declspec(align(4)) DIALOG_BUFFER[1024];


#if 0
enum dialog_numbers
{
	DIALOG_GROUP_PADDING = 4,
	DIALOG_ITEM_HEIGHT = 14,
	DIALOG_BUTTON_WIDTH_SMALL = 14,
	DIALOG_BUTTON_WIDTH_NORMAL = 50,
	DIALOG_BUTTON_WIDTH_LARGE = 90,

};

typedef enum
{
	ITEMTYPE_COMBOBOX,
	ITEMTYPE_STATIC,
	ITEMTYPE_,
} item_type;

typedef struct
{
	int X1;
	int Y1;
	int X2;
	int Y2;
} item_bounds;

typedef struct
{
	char* Caption;

} item_button;

typedef struct
{
	item_type Type;
	item_bounds Bounds;
	union
	{
		item_button Button;

	};
} dialog_item;

typedef struct
{
	char* Title;
	char* Font;
	WORD FontSize;
	dialog_item Groups[];
} dialog_info;

static win32_dialog DIALOG_MISC_ABOUT =
{
	L"About NEMU"

}

static win32_dialog DIALOG_CONFIG_VIDEO =
{
	.Title = "Video Configurations",
	.Font = "Segoe UI",
	.FontSize = 9,
	.Groups[0] =
	{

	},
};

static BYTE*
Config__DoDialogItem(BYTE* Data, LPCSTR Text, WORD Id, WORD Control, DWORD Style, int x, int y, int w, int h)
dialog_create_item(dialog_item DialogItem, BYTE* Data)
{
	Data = pointer_align(Data, sizeof(DWORD));
	DLGITEMTEMPLATEW* ItemTemplate = (DLGITEMTMEPLATEW*)Data;
	ItemTemplate->style = Style | WS_CHILD | WS_VISIBLE;
	ItemTemplate->x = DialogItem.X;
	ItemTemplate->y = Y + ((Control == CONTROL_STATIC) ? 2 : 0);
	ItemTemplate->cx = Width;
	ItemTemplate->cy = Height - ((Control == CONTROL_EDIT) ? 2 : 0) - ((Control == CONTROL_STATIC) ? 2 : 0);
	ItemTemplate->id = Identifier;
	Data += sizeof(DLGITEMTEMPLATE);

	Data = pointer_align(Data, sizeof(WORD));
	WORD* WindowClass = Data;
	WindowClass[0] = 0xffff;
	WindowClass[1] = DialogItem.Type;
	Data += sizeof(*WindowClass) * 2;

	Data = pointer_align(Data, sizeof(WCHAR));
	DWORD ItemTextCharacters = MultiByteToWideChar(CP_UTF8, 0, Text, -1, (WCHAR*)Data, 128);
	Data += ItemTextCharacters * sizeof(WCHAR);

	Data = pointer_align(Data, sizeof(WORD));
	WORD* Extra = Data;
	*Extra = 0;
	Data += sizeof(*Extra);
	
	BYTE* Result = Data;
	return Result;
}

static void
dialog_create(dialog_info* DialogInfo)
{
	static BYTE __declspec(align(4)) DialogBuffer[1024];
	DLGTEMPLATEW* DialogTemplate = DialogData;
	DialogTemplate->style = DS_SETFONT | DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU;
	DialogTemplate->cdit = 0;
	DialogTemplate->cx = PADDING + COL10W + PADDING + COL11W + PADDING;
	DialogTemplate->cy = PADDING + ROW0H + PADDING + ROW1H + PADDING + ROW2H + ITEM_HEIGHT;
	BYTE* DialogData = DialogBuffer + sizeof(*DialogTemplate);
	DialogData = pointer_align(DialogData, sizeof(WCHAR));
	WCHAR* Menu = DialogData;
	*Menu = 0;
	DialogData += sizeof(*Menu);
	DialogData = pointer_align(DialogData, sizeof(WCHAR));
	WCHAR* WindowClass = DialogData;
	*WindowClass = 0;
	DialogData += sizeof(*WindowClass);
	DialogData = pointer_align(DialogData, sizeof(WCHAR));
	DWORD TitleCharacters = MultiByteToWideChar(CP_UTF8, 0, DialogInfo->Title, -1, (WCHAR*)DialogData, 128);
	DialogData += TitleCharacters * sizeof(WCHAR);
	DialogData = pointer_align(DialogData, sizeof(WORD));
	WORD* FontSize = DialogData;
	*FontSize = DialogInfo->FontSize;
	DialogData += sizeof(*FontSize);
	DialogData = pointer_align(DialogData, sizeof(WCHAR));
	DWORD FontNameCharacters = MultiByteToWideChar(CP_UTF8, 0, DialogInfo->FontName, -1, (WCHAR*)DialogData, 128);
	DialogData += FontNameCharacters * sizeof(WCHAR);

	for(int ItemIndex = 0; ItemIndex < DialogInfo->NumItems; ItemIndex += 1)
	{
		dialog_item Item = DialogInfo->Items[ItemIndex];
		DialogData = dialog_create_item(Item, DialogData);
		NumItems -= 1;
	}

	int ButtonX = PADDING + COL10W + COL11W + PADDING - 3 * (PADDING + BUTTON_WIDTH);
	int ButtonY = PADDING + ROW0H + ROW1H + ROW2H + PADDING;

	DLGITEMTEMPLATE* OkData = pointer_align(Data, sizeof(DWORD));
	Data = Config__DoDialogItem(Data, "OK", ID_OK, CONTROL_BUTTON, WS_TABSTOP | BS_DEFPUSHBUTTON, ButtonX, ButtonY, BUTTON_WIDTH, ITEM_HEIGHT);
	ButtonX += BUTTON_WIDTH + PADDING;

	DLGITEMTEMPLATE* CancelData = Config__Align(Data, sizeof(DWORD));
	Data = Config__DoDialogItem(Data, "Cancel", ID_CANCEL, CONTROL_BUTTON, WS_TABSTOP | BS_PUSHBUTTON, ButtonX, ButtonY, BUTTON_WIDTH, ITEM_HEIGHT);
	ButtonX += BUTTON_WIDTH + PADDING;

	DLGITEMTEMPLATE* DefaultsData = Config__Align(Data, sizeof(DWORD));
	Data = Config__DoDialogItem(Data, "Defaults", ID_DEFAULTS, CONTROL_BUTTON, WS_TABSTOP | BS_PUSHBUTTON, ButtonX, ButtonY, BUTTON_WIDTH, ITEM_HEIGHT);
	ButtonX += BUTTON_WIDTH + PADDING;

	size_t GroupIndex = 0;
	for(const Config__DialogGroup* Group = Layout->Groups; Group->Caption; Group++)
	{
		//Config__DialogGroup Group = 
		int X = Group->Rect.Left + PADDING;
		int Y = Group->Rect.Top + PADDING;
		int W = Group->Rect.Width;
		int H = Group->Rect.Height;

		Data = Config__DoDialogItem(Data, Group->Caption, -1, CONTROL_BUTTON, BS_GROUPBOX, X, Y, W, H);
		NumItems += 1;

		X += PADDING;
		Y += ITEM_HEIGHT - PADDING;
		W -= 2 * PADDING;

		for (const Config__DialogItem* Item = Group->Items; Item->Text; Item++)
		{
			int HasCheckbox  = !!(Item->Item & ITEM_CHECKBOX);
			int HasNumber    = !!(Item->Item & ITEM_NUMBER);
			int HasCombobox  = !!(Item->Item & ITEM_COMBOBOX);
			int OnlyCheckbox = !(Item->Item & ~ITEM_CHECKBOX);
			int HasHotKey    = !!(Item->Item & ITEM_HOTKEY);

			int ItemX = X;
			int ItemW = W;
			int ItemId = Item->Id;

			if (HasCheckbox)
			{
				if (!OnlyCheckbox)
				{
					// reduce width so checbox can fit other control on the right
					ItemW = Item->Width;
				}
				Data = Config__DoDialogItem(Data, Item->Text, ItemId, CONTROL_BUTTON, WS_TABSTOP | BS_AUTOCHECKBOX, ItemX, Y, ItemW, ITEM_HEIGHT);
				NumItems += 1;
				ItemId++;
				if (!OnlyCheckbox)
				{
					ItemX += Item->Width + PADDING;
					ItemW = W - (Item->Width + PADDING);
				}
			}

			if ((HasCombobox && !HasCheckbox) || (HasNumber || HasHotKey) && !HasCheckbox)
			{
				// label, only for controls without checkbox, or combobox
				Data = Config__DoDialogItem(Data, Item->Text, -1, CONTROL_STATIC, 0, ItemX, Y, Item->Width, ITEM_HEIGHT);
				NumItems += 1;
				ItemX += Item->Width + PADDING;
				ItemW -= Item->Width + PADDING;
			}

			if (HasNumber)
			{
				Data = Config__DoDialogItem(Data, "", ItemId, CONTROL_EDIT, WS_TABSTOP | WS_BORDER | ES_RIGHT | ES_NUMBER, ItemX, Y, ItemW, ITEM_HEIGHT);
				NumItems += 1;
			}

			if (HasHotKey)
			{
				Data = Config__DoDialogItem(Data, "", ItemId, CONTROL_BUTTON, WS_TABSTOP, ItemX, Y, ItemW, ITEM_HEIGHT);
				NumItems += 1;
			}

			if (HasCombobox)
			{
				Data = Config__DoDialogItem(Data, "", ItemId, CONTROL_COMBOBOX, WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS, ItemX, Y, ItemW, ITEM_HEIGHT);
				NumItems += 1;
			}

			if (Item->Item & ITEM_FOLDER)
			{
				Data = Config__DoDialogItem(Data, "", ItemId, CONTROL_EDIT, WS_TABSTOP | WS_BORDER, X, Y, W - BUTTON_SMALL_WIDTH - PADDING + 2, ITEM_HEIGHT);
				NumItems += 1;
				ItemId++;
				Data = Config__DoDialogItem(Data, "...", ItemId, CONTROL_BUTTON, WS_TABSTOP | BS_PUSHBUTTON, X + W - BUTTON_SMALL_WIDTH + 2, Y + 1, BUTTON_SMALL_WIDTH - 4, ITEM_HEIGHT - 3);
				NumItems += 1;
			}
			Y += ITEM_HEIGHT;
		}
	}
	Assert(DialogData <= (DialogBuffer + array_length(DialogBuffer)));
	//DialogBoxIndirectParamW(GetModuleHandleW(NULL), (DGLTEMPLATEW*)Data, 0, dialog_proc, (LPARAM)Config);
}

#endif
