#pragma once
#include <windows.h>

class Symbol {
public:
    Symbol();
    Symbol(char c, WORD color, int x, int y);

    char getChar() const;
    WORD getColor() const;
    int  getX()   const;
    int  getY()   const;

    void setChar(char c);
    void setColor(WORD color);
    void setPosition(int x, int y);

    void draw()  const;
    void erase() const;

    static char getRandomPrintableChar();

private:
    char character;
    WORD color;
    int  x, y;
};