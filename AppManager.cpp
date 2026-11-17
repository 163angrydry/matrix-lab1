#include "AppManager.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <windows.h>

AppManager::AppManager(int argc, char* argv[]) : running(true),
    secondStart(0), nextSpawnIndex(0) {
    srand((unsigned)time(nullptr));

    if (argc > 1) {
        std::string first = argv[1];
        if (first == "--help" || first == "/?") {
            showHelp();
            running = false;
            return;
        }
        if (!parseArgs(argc, argv)) running = false;
    } else {
        runDialog();
    }
}

bool AppManager::parseArgs(int argc, char* argv[]) {
    if (argc != 8) {
        std::cerr << "Error: expected 7 arguments\n";
        showHelp();
        return false;
    }
    try {
        int frequency = std::stoi(argv[1]);
        int speed     = std::stoi(argv[2]);
        int length    = std::stoi(argv[3]);
        std::string epi = argv[4];
        int explosionProb = std::stoi(argv[5]);
        int minRadius = std::stoi(argv[6]);
        int maxRadius = std::stoi(argv[7]);

        if (!validateFrequency(frequency) || !validateSpeed(speed) ||
            !validateLength(length) || !validateEpilepsy(epi) ||
            !validateExplosionProbability(explosionProb) ||
            !validateMinRadius(minRadius) ||
            !validateMaxRadius(minRadius, maxRadius))
            return false;

        params.frequency = frequency;
        params.speed     = speed;
        params.length    = length;
        params.epilepsy  = (epi == "Y" || epi == "y");
        params.explosionProbability = explosionProb;
        params.minExplosionRadius = minRadius;
        params.maxExplosionRadius = maxRadius;
        return true;
    } catch (...) {
        std::cerr << "Error: invalid arguments\n";
        return false;
    }
}

void AppManager::showHelp() {
    std::cout << "Matrix - OOP Lab 3\n"
              << "Usage: matrix.exe [frequency] [speed] [length] [epilepsy] [explosionProb] [minRadius] [maxRadius]\n"
              << "  frequency    : 1..30 (lines per second)\n"
              << "  speed        : 1..30 (chars per second)\n"
              << "  length       : 1..30 (chars)\n"
              << "  epilepsy     : Y or N\n"
              << "  explosionProb: 1..1000 (probability per step)\n"
              << "  minRadius    : 1..10\n"
              << "  maxRadius    : 1..10 (>= minRadius)\n"
              << "Example: matrix.exe 3 23 8 N 50 2 5\n"
              << "Or run without arguments for dialog mode.\n";
}

void AppManager::runDialog() {
    int frequency;
    do {
        std::cout << "Enter frequency (1-30): ";
        std::cin >> frequency;
        if (!validateFrequency(frequency)) std::cout << "Invalid frequency. Try again.\n";
    } while (!validateFrequency(frequency));

    int speed;
    do {
        std::cout << "Enter speed (1-30): ";
        std::cin >> speed;
        if (!validateSpeed(speed)) std::cout << "Invalid speed. Try again.\n";
    } while (!validateSpeed(speed));

    int length;
    do {
        std::cout << "Enter length (1-30): ";
        std::cin >> length;
        if (!validateLength(length)) std::cout << "Invalid length. Try again.\n";
    } while (!validateLength(length));

    std::string epi;
    do {
        std::cout << "Epilepsy mode (Y/N): ";
        std::cin >> epi;
        if (!validateEpilepsy(epi)) std::cout << "Invalid input. Try again.\n";
    } while (!validateEpilepsy(epi));

    int explosionProb;
    do {
        std::cout << "Explosion probability (1-1000): ";
        std::cin >> explosionProb;
        if (!validateExplosionProbability(explosionProb)) std::cout << "Invalid probability. Try again.\n";
    } while (!validateExplosionProbability(explosionProb));

    int minRadius;
    do {
        std::cout << "Min explosion radius (1-10): ";
        std::cin >> minRadius;
        if (!validateMinRadius(minRadius)) std::cout << "Invalid min radius. Try again.\n";
    } while (!validateMinRadius(minRadius));

    int maxRadius;
    do {
        std::cout << "Max explosion radius (1-10, >= " << minRadius << "): ";
        std::cin >> maxRadius;
        if (!validateMaxRadius(minRadius, maxRadius)) std::cout << "Invalid max radius. Try again.\n";
    } while (!validateMaxRadius(minRadius, maxRadius));

    params.frequency = frequency;
    params.speed     = speed;
    params.length    = length;
    params.epilepsy  = (epi == "Y" || epi == "y");
    params.explosionProbability = explosionProb;
    params.minExplosionRadius = minRadius;
    params.maxExplosionRadius = maxRadius;
}

bool AppManager::validateFrequency(int f) { return f >= 1 && f <= 30; }
bool AppManager::validateSpeed(int s)     { return s >= 1 && s <= 30; }
bool AppManager::validateLength(int l)    { return l >= 1 && l <= 30; }
bool AppManager::validateEpilepsy(const std::string& s) {
    return s == "Y" || s == "y" || s == "N" || s == "n";
}
bool AppManager::validateExplosionProbability(int p) { return p >= 1 && p <= 1000; }
bool AppManager::validateMinRadius(int r) { return r >= 1 && r <= 10; }
bool AppManager::validateMaxRadius(int rMin, int rMax) {
    return rMax >= 1 && rMax <= 10 && rMax >= rMin;
}

void AppManager::run() {
    if (!running) return;
    ConsoleHelper::clearScreen();
    ConsoleHelper::hideCursor();
    mainLoop();
    ConsoleHelper::showCursor();
    ConsoleHelper::setColor(7);
    ConsoleHelper::setCursorPosition(0, 0);
}

void AppManager::regenerateSchedule() {
    secondStart = GetTickCount();
    spawnOffsets.clear();

    for (int i = 0; i < params.frequency; ++i) {
        spawnOffsets.push_back(rand() % 1001);
    }

    std::sort(spawnOffsets.begin(), spawnOffsets.end());
    nextSpawnIndex = 0;
}

void AppManager::spawnNewLine(int width, int height) {
    Direction dir   = Direction::TopToBottom;
    LineShape shape = LineShape::Straight;

    auto line = std::make_unique<Line>(
        params.length, dir, shape, params.epilepsy,
        params.explosionProbability,
        params.minExplosionRadius,
        params.maxExplosionRadius
    );
    line->reset(width, height);
    activeFigures.push_back(std::move(line));
}

void AppManager::updateAllFigures() {
    std::vector<std::unique_ptr<Figure>> newFigures;

    for (auto& fig : activeFigures) {
        fig->step();

        Line* line = dynamic_cast<Line*>(fig.get());
        if (line && line->hasExploded() && !line->isExplosionHandled() && line->getLength() > 0) {
            auto explosion = std::make_unique<Explosion>(
                line->getHeadX(), line->getHeadY(),
                params.minExplosionRadius, params.maxExplosionRadius
            );
            explosion->draw();
            newFigures.push_back(std::move(explosion));
            line->markExplosionHandled();
        }
    }

    for (auto& f : newFigures) {
        activeFigures.push_back(std::move(f));
    }

    for (auto& fig : activeFigures) {
        Line* line = dynamic_cast<Line*>(fig.get());
        if (line && !line->isFinished()) {
            line->draw();
        }
    }
}

void AppManager::removeFinishedFigures() {
    auto it = activeFigures.begin();
    while (it != activeFigures.end()) {
        if ((*it)->isFinished()) {
            (*it)->erase();
            it = activeFigures.erase(it);
        } else {
            ++it;
        }
    }
}

void AppManager::mainLoop() {
    int width, height;
    ConsoleHelper::getConsoleSize(width, height);

    regenerateSchedule();

    DWORD stepInterval = 1000 / params.speed;
    DWORD lastStepTime = GetTickCount();

    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        DWORD now = GetTickCount();

        if (now - secondStart >= 1000) {
            regenerateSchedule();
        }

        if (nextSpawnIndex < spawnOffsets.size()) {
            DWORD elapsed = now - secondStart;
            if (elapsed >= spawnOffsets[nextSpawnIndex]) {
                spawnNewLine(width, height);
                nextSpawnIndex++;
            }
        }

        if (now - lastStepTime >= stepInterval) {
            lastStepTime = now;
            updateAllFigures();
            removeFinishedFigures();
        }

        Sleep(10);
    }
}