#include "Line.h"
#include "ConsoleHelper.h"
#include <cstdlib>
#include <cmath>

Line::Line(int length, Direction dir, LineShape shape, bool epilepsy,
    int explosionProbability, int minExplosionRadius, int maxExplosionRadius)
    : length(length), targetLength(length), direction(dir), shape(shape), epilepsy(epilepsy),
    explosionProbability(explosionProbability),
    minExplosionRadius(minExplosionRadius),
    maxExplosionRadius(maxExplosionRadius),
    exploded(false),
    explosionHandled(false),
    headX(0), headY(0), stepCount(0), finished(false),
    consoleWidth(80), consoleHeight(25) {}

int Line::getOffsetX(int index, int size) const {
    switch (shape) {
    case LineShape::Straight:
        return 0;
    case LineShape::Zigzag:
        if (direction == Direction::TopToBottom || direction == Direction::BottomToTop)
            return (index % 2 == 0) ? 0 : 1;
        return 0;
    case LineShape::Diamond:
        if (direction == Direction::TopToBottom || direction == Direction::BottomToTop) {
            int mid = size / 2;
            return mid - std::abs(index - mid);
        }
        return 0;
    }
    return 0;
}

void Line::getSymbolPos(int index, int& x, int& y, int hx, int hy, int size) const {
    int offX = getOffsetX(index, size);
    switch (direction) {
    case Direction::TopToBottom:
        x = hx + offX;
        y = hy - index;
        break;
    case Direction::BottomToTop:
        x = hx + offX;
        y = hy + index;
        break;
    case Direction::LeftToRight:
        x = hx - index;
        y = hy;
        break;
    case Direction::RightToLeft:
        x = hx + index;
        y = hy;
        break;
    }
}

void Line::reset(int consoleWidth, int consoleHeight) {
    this->consoleWidth = consoleWidth;
    this->consoleHeight = consoleHeight;
    finished = false;
    exploded = false;
    explosionHandled = false;
    stepCount = 0;
    targetLength = length;
    chars.clear();

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
    if (finished) return;

    std::vector<char> oldChars = chars;
    int prevHeadX = headX;
    int prevHeadY = headY;

    if (!exploded && stepCount > 0) {
        int hx, hy;
        getSymbolPos(0, hx, hy, headX, headY, (int)chars.size());
        if (hx >= 0 && hx < consoleWidth && hy >= 0 && hy < consoleHeight) {
            if (rand() % 1000 < explosionProbability) {
                exploded = true;
            }
        }
    }

    if (exploded && targetLength > 0) {
        targetLength--;
    }

    switch (direction) {
    case Direction::TopToBottom:  headY++; break;
    case Direction::BottomToTop:  headY--; break;
    case Direction::LeftToRight:  headX++; break;
    case Direction::RightToLeft:  headX--; break;
    }
    stepCount++;

    if (targetLength > 0) {
        chars.insert(chars.begin(), Symbol::getRandomPrintableChar());
        if ((int)chars.size() > targetLength) {
            chars.resize(targetLength);
        }
    }
    else {
        chars.clear();
    }

    for (int i = 0; i < (int)oldChars.size(); ++i) {
        int x, y;
        getSymbolPos(i, x, y, prevHeadX, prevHeadY, (int)oldChars.size());
        if (x >= 0 && x < consoleWidth && y >= 0 && y < consoleHeight)
            ConsoleHelper::writeCharAt(x, y, ' ', 0);
    }

    for (int i = 0; i < (int)chars.size(); ++i) {
        int x, y;
        getSymbolPos(i, x, y, headX, headY, (int)chars.size());
        if (x >= 0 && x < consoleWidth && y >= 0 && y < consoleHeight) {
            WORD color = epilepsy ? ConsoleHelper::getRandomBrightColor()
                : ConsoleHelper::getGreenColor();
            ConsoleHelper::writeCharAt(x, y, chars[i], color);
        }
    }

    if (chars.empty()) {
        finished = true;
        return;
    }

    bool allOut = false;
    int sz = (int)chars.size();
    switch (direction) {
    case Direction::TopToBottom:
        allOut = (headY - (sz - 1) >= consoleHeight);
        break;
    case Direction::BottomToTop:
        allOut = (headY + (sz - 1) < 0);
        break;
    case Direction::LeftToRight:
        allOut = (headX - (sz - 1) >= consoleWidth);
        break;
    case Direction::RightToLeft:
        allOut = (headX + (sz - 1) < 0);
        break;
    }

    if (allOut) {
        finished = true;
    }
}

void Line::draw() {
    for (int i = 0; i < (int)chars.size(); ++i) {
        int x, y;
        getSymbolPos(i, x, y, headX, headY, (int)chars.size());
        if (x >= 0 && x < consoleWidth && y >= 0 && y < consoleHeight) {
            WORD color = epilepsy ? ConsoleHelper::getRandomBrightColor()
                : ConsoleHelper::getGreenColor();
            ConsoleHelper::writeCharAt(x, y, chars[i], color);
        }
    }
}

void Line::eraseAll() {
    for (int i = 0; i < (int)chars.size(); ++i) {
        int x, y;
        getSymbolPos(i, x, y, headX, headY, (int)chars.size());
        if (x >= 0 && x < consoleWidth && y >= 0 && y < consoleHeight) {
            ConsoleHelper::writeCharAt(x, y, ' ', 0);
        }
    }
}

void Line::erase() {
    eraseAll();
}

bool Line::isFinished() const { return finished; }