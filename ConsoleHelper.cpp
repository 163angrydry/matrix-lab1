#include "ConsoleHelper.h"
#include <cstdlib>
#include <cstdio>   // ← ДОБАВЬТЕ ЭТУ СТРОКУ

void ConsoleHelper::clearScreen() { system("cls"); }

void ConsoleHelper::setCursorPosition(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void ConsoleHelper::setColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ConsoleHelper::hideCursor() {
    CONSOLE_CURSOR_INFO info{ 100, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void ConsoleHelper::showCursor() {
    CONSOLE_CURSOR_INFO info{ 100, TRUE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void ConsoleHelper::getConsoleSize(int& width, int& height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void ConsoleHelper::writeCharAt(int x, int y, char c, WORD color) {
    if (x < 0 || y < 0) return;
    setCursorPosition(x, y);
    setColor(color);
    putchar(c);   // ← ПРОВЕРЬТЕ: putchar, а НЕ putcher
}

WORD ConsoleHelper::getRandomBrightColor() {
    WORD bright[] = { 9, 10, 11, 12, 13, 14, 15 };
    return bright[rand() % 7];
}

WORD ConsoleHelper::getGreenColor() { return 10; }
