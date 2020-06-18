#include <windows.h>
#include "Interact.hpp"

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

namespace Interact {
	bool isSupportAnsi;

	bool OpenANSIControlChar()
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE) { return false; }

		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode)) { return false; }

		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwMode)) { return false; }
		return true;
	}

	BOOL SetConsoleColor(const WORD wAttributes)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsole == INVALID_HANDLE_VALUE)
			return FALSE;

		return SetConsoleTextAttribute(hConsole, wAttributes);
	}

	bool SetConsoleColorRed()
	{
		return SetConsoleColor(FOREGROUND_RED);
	}

	bool SetConsoleColorWhite()
	{
		return SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	bool SetConsoleColorYellow()
	{
		return SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	void SetRed()
	{
		if (!isSupportAnsi)
		{
			SetConsoleColorRed();
		}
		else
		{
			std::cerr << "\033[1;31m";
		}
	}

	void SetYellow()
	{
		if (!isSupportAnsi)
		{
			SetConsoleColorYellow();
		}
		else
		{
			std::cerr << "\033[1;33m";
		}
	}

	void SetWhite()
	{
		if (!isSupportAnsi)
		{
			SetConsoleColorWhite();
		}
		else
		{
			std::cout << "\033[0;37m";
		}
	}

	void InitIOEnvironment()
	{
		std::locale::global(std::locale(""));
		isSupportAnsi = OpenANSIControlChar();
	}

	void werror(const std::wstring& msg)
	{
		SetRed();
		std::wcerr << msg << std::endl;
		SetWhite();
	}
}
