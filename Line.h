#pragma once
#include "Symbol.h"
#include <vector>

enum class Direction { TopToBottom, RightToLeft, BottomToTop, LeftToRight };
enum class Shape { Straight, Zigzag, Diamond };

class Line {
public:
    Line(int length, Direction dir, Shape shape, bool epilepsy);

    void step();
    bool isFinished() const;
    void reset(int consoleWidth, int consoleHeight);

private:
    int  length;
    Direction direction;
    Shape     shape;
    bool      epilepsy;

    int  headX, headY;
    int  stepCount;
    bool finished;
    int  consoleWidth, consoleHeight;

    int getOffsetX(int index) const;
    int getOffsetY(int index) const;
    void getSymbolPos(int index, int& x, int& y, int hx, int hy) const;
};