#include "Line.h"
#include "ConsoleHelper.h"
#include <cstdlib>
#include <cmath>

Line::Line(int length, Direction dir, Shape shape, bool epilepsy)
    : length(length), direction(dir), shape(shape), epilepsy(epilepsy),
    headX(0), headY(0), stepCount(0), finished(false),
    consoleWidth(80), consoleHeight(25) {}

int Line::getOffsetX(int index) const {
    switch (shape) {
    case Shape::Straight:
        return 0;
    case Shape::Zigzag:
        if (direction == Direction::TopToBottom || direction == Direction::BottomToTop)
            return (index % 2 == 0) ? 0 : 1;
        return 0;
    case Shape::Diamond:
        if (direction == Direction::TopToBottom || direction == Direction::BottomToTop) {
            int mid = length / 2;
            return mid - std::abs(index - mid);
        }
        return 0;
    }
    return 0;
}

int Line::getOffsetY(int index) const {
    switch (direction) {
    case Direction::TopToBottom:  return -index;
    case Direction::BottomToTop:  return  index;
    default: return 0;
    }
}

void Line::getSymbolPos(int index, int& x, int& y, int hx, int hy) const {
    switch (direction) {
    case Direction::TopToBottom:
        x = hx + getOffsetX(index);
        y = hy - index;
        break;
    case Direction::BottomToTop:
        x = hx + getOffsetX(index);
        y = hy + index;
        break;
    case Direction::LeftToRight:
        x = hx - index;
        y = hy + getOffsetY(index);
        break;
    case Direction::RightToLeft:
        x = hx + index;
        y = hy + getOffsetY(index);
        break;
    }
}

void Line::reset(int consoleWidth, int consoleHeight) {
    this->consoleWidth = consoleWidth;
    this->consoleHeight = consoleHeight;
    finished = false;
    stepCount = 0;

    ConsoleHelper::clearScreen();

    switch (direction) {
    case Direction::TopToBottom:
        headX = rand() % consoleWidth;
        headY = -length;
        break;
    case Direction::BottomToTop:
        headX = rand() % consoleWidth;
        headY = consoleHeight + length;
        break;
    case Direction::LeftToRight:
        headX = -length;
        headY = rand() % consoleHeight;
        break;
    case Direction::RightToLeft:
        headX = consoleWidth + length;
        headY = rand() % consoleHeight;
        break;
    }
}

void Line::step() {
   
    if (stepCount >= length) {
        int prevHeadX = headX, prevHeadY = headY;
       
        switch (direction) {
        case Direction::TopToBottom:  prevHeadY--; break;
        case Direction::BottomToTop:  prevHeadY++; break;
        case Direction::LeftToRight:  prevHeadX--; break;
        case Direction::RightToLeft:  prevHeadX++; break;
        }
        int tx, ty;
        getSymbolPos(length - 1, tx, ty, prevHeadX, prevHeadY);
        if (tx >= 0 && tx < consoleWidth && ty >= 0 && ty < consoleHeight)
            ConsoleHelper::writeCharAt(tx, ty, ' ', 0);
    }

   
    switch (direction) {
    case Direction::TopToBottom:  headY++; break;
    case Direction::BottomToTop:  headY--; break;
    case Direction::LeftToRight:  headX++; break;
    case Direction::RightToLeft:  headX--; break;
    }
    stepCount++;

    int hx, hy;
    getSymbolPos(0, hx, hy, headX, headY);
    if (hx >= 0 && hx < consoleWidth && hy >= 0 && hy < consoleHeight) {
        WORD color = epilepsy ? ConsoleHelper::getRandomBrightColor()
            : ConsoleHelper::getGreenColor();
        ConsoleHelper::writeCharAt(hx, hy, Symbol::getRandomPrintableChar(), color);
    }

    bool allOut = false;
    switch (direction) {
    case Direction::TopToBottom:
        allOut = (headY - (length - 1) >= consoleHeight);
        break;
    case Direction::BottomToTop:
        allOut = (headY + (length - 1) < 0);
        break;
    case Direction::LeftToRight:
        allOut = (headX - (length - 1) >= consoleWidth);
        break;
    case Direction::RightToLeft:
        allOut = (headX + (length - 1) < 0);
        break;
    }

    if (allOut && stepCount >= length) {
        int lastX, lastY;
        getSymbolPos(length - 1, lastX, lastY, headX, headY);
        if (lastX >= 0 && lastX < consoleWidth && lastY >= 0 && lastY < consoleHeight)
            ConsoleHelper::writeCharAt(lastX, lastY, ' ', 0);
    }

    finished = allOut;
}

bool Line::isFinished() const { return finished; }