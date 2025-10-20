#pragma once
#include <iostream>
#include <chrono>
#include <Windows.h>

class console_color
{
public:
	static void SetColor(int text, int background) {
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
	};
};