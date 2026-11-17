#pragma once
#include "Figure.h"
#include "Symbol.h"
#include <vector>

enum class Direction { TopToBottom, RightToLeft, BottomToTop, LeftToRight };
enum class LineShape { Straight, Zigzag, Diamond };

class Line : public Figure {
public:
    Line(int length, Direction dir, LineShape shape, bool epilepsy,
        int explosionProbability, int minExplosionRadius, int maxExplosionRadius);

    void draw() override;
    void step() override;
    bool isFinished() const override;
    void erase() override;

    void reset(int consoleWidth, int consoleHeight);
    void eraseAll();

    int getHeadX() const { return headX; }
    int getHeadY() const { return headY; }
    bool hasExploded() const { return exploded; }
    bool isExplosionHandled() const { return explosionHandled; }
    int getLength() const { return (int)chars.size(); }

    void markExplosionHandled() { explosionHandled = true; }

private:
    int length;
    int targetLength;
    Direction direction;
    LineShape shape;
    bool epilepsy;

    int explosionProbability;
    int minExplosionRadius;
    int maxExplosionRadius;
    bool exploded;
    bool explosionHandled;

    std::vector<char> chars;

    int headX, headY;
    int stepCount;
    bool finished;
    int consoleWidth, consoleHeight;

    int getOffsetX(int index, int size) const;
    void getSymbolPos(int index, int& x, int& y, int hx, int hy, int size) const;
};