#include "Symbol.h"
#include "ConsoleHelper.h"
#include <cstdlib>

Symbol::Symbol() : character(' '), color(10), x(0), y(0) {}
Symbol::Symbol(char c, WORD color, int x, int y)
    : character(c), color(color), x(x), y(y) {}

char Symbol::getChar() const { return character; }
WORD Symbol::getColor() const { return color; }
int  Symbol::getX()   const { return x; }
int  Symbol::getY()   const { return y; }

void Symbol::setChar(char c) { character = c; }
void Symbol::setColor(WORD color) { this->color = color; }
void Symbol::setPosition(int x, int y) { this->x = x; this->y = y; }

void Symbol::draw() const { ConsoleHelper::writeCharAt(x, y, character, color); }
void Symbol::erase() const { ConsoleHelper::writeCharAt(x, y, ' ', 0); }

char Symbol::getRandomPrintableChar() {
    return (char)(33 + rand() % 94);
}