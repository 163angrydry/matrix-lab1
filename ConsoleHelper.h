#pragma once
#include <windows.h>

class ConsoleHelper {
public:
    static void clearScreen();
    static void setCursorPosition(int x, int y);
    static void setColor(WORD color);
    static void hideCursor();
    static void showCursor();
    static void getConsoleSize(int& width, int& height);
    static void writeCharAt(int x, int y, char c, WORD color);
    static WORD getRandomBrightColor();
    static WORD getGreenColor();
};