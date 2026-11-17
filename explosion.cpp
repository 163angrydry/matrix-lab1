#include "Explosion.h"
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Explosion::Explosion(int centerX, int centerY, int minRadius, int maxRadius)
    : centerX(centerX), centerY(centerY),
    currentRadius(minRadius),
    previousRadius(minRadius),
    maxRadius(maxRadius),
    finished(false),
    lastStepTime(GetTickCount()) {

    drawRing(currentRadius);
}

void Explosion::eraseRing(int radius) {
    for (const auto& p : currentRingPixels) {
        ConsoleHelper::writeCharAt(p.x, p.y, ' ', 0);
    }
    currentRingPixels.clear();
}

void Explosion::drawRing(int radius) {
    int width, height;
    ConsoleHelper::getConsoleSize(width, height);

    int steps = radius * 12;
    if (steps < 24) steps = 24;

    for (int i = 0; i < steps; ++i) {
        double angle = 2.0 * M_PI * i / steps;
        int x = centerX + static_cast<int>(std::round(radius * 2.0 * std::cos(angle)));
        int y = centerY + static_cast<int>(std::round(radius * std::sin(angle)));

        if (x >= 0 && x < width && y >= 0 && y < height) {
            char c = static_cast<char>(33 + rand() % 94);
            WORD color = static_cast<WORD>(9 + rand() % 7);
            ConsoleHelper::writeCharAt(x, y, c, color);
            currentRingPixels.push_back({ x, y });
        }
    }
}

void Explosion::draw() {
}

void Explosion::step() {
    DWORD now = GetTickCount();
    if (now - lastStepTime >= 200) {
        lastStepTime = now;

        eraseRing(previousRadius);

        currentRadius++;
        previousRadius = currentRadius;

        if (currentRadius > maxRadius) {
            finished = true;
            eraseRing(previousRadius);
        }
        else {
            drawRing(currentRadius);
        }
    }
}

bool Explosion::isFinished() const {
    return finished;
}

void Explosion::erase() {
    eraseRing(previousRadius);
}