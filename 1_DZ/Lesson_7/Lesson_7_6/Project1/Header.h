#pragma once
#include <chrono>
#include <iostream>
#include <Windows.h>

class consol_color
{
public:
    static void SetColor(int text, int background)
    {
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
    }
    static void SetPosition(int x, int y)
    {
        COORD point;
        point.X = x;
        point.Y = y;
        SetConsoleCursorPosition(hStdOut, point);
    }
private:
    static HANDLE hStdOut;
};
HANDLE consol_color::hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);