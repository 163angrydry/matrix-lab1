#pragma once
#include "Figure.h"
#include "ConsoleHelper.h"
#include <vector>
#include <windows.h>

class Explosion : public Figure {
public:
    Explosion(int centerX, int centerY, int minRadius, int maxRadius);

    void draw() override;
    void step() override;
    bool isFinished() const override;
    void erase() override;

private:
    int centerX;
    int centerY;
    int currentRadius;
    int previousRadius;
    int maxRadius;
    bool finished;
    DWORD lastStepTime;

    struct Pixel {
        int x, y;
    };
    std::vector<Pixel> currentRingPixels;

    void drawRing(int radius);
    void eraseRing(int radius);
};