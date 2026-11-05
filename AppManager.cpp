#include "AppManager.h"
#include "Line.h"
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
    }
    else {
        runDialog();
    }
}

bool AppManager::parseArgs(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Error: expected 4 arguments\n";
        showHelp();
        return false;
    }
    try {
        int frequency = std::stoi(argv[1]);
        int speed = std::stoi(argv[2]);
        int length = std::stoi(argv[3]);
        std::string epi = argv[4];

        if (!validateFrequency(frequency) || !validateSpeed(speed) ||
            !validateLength(length) || !validateEpilepsy(epi))
            return false;

        params.frequency = frequency;
        params.speed = speed;
        params.length = length;
        params.epilepsy = (epi == "Y" || epi == "y");
        return true;
    }
    catch (...) {
        std::cerr << "Error: invalid arguments\n";
        return false;
    }
}

void AppManager::showHelp() {
    std::cout << "Matrix - OOP Lab 2\n"
        << "Usage: matrix.exe [frequency] [speed] [length] [epilepsy]\n"
        << "  frequency : 1..30 (lines per second)\n"
        << "  speed     : 1..30 (chars per second)\n"
        << "  length    : 1..30 (chars)\n"
        << "  epilepsy  : Y or N\n"
        << "Example: matrix.exe 3 23 8 Y\n"
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

    params.frequency = frequency;
    params.speed = speed;
    params.length = length;
    params.epilepsy = (epi == "Y" || epi == "y");
}

bool AppManager::validateFrequency(int f) { return f >= 1 && f <= 30; }
bool AppManager::validateSpeed(int s) { return s >= 1 && s <= 30; }
bool AppManager::validateLength(int l) { return l >= 1 && l <= 30; }
bool AppManager::validateEpilepsy(const std::string& s) {
    return s == "Y" || s == "y" || s == "N" || s == "n";
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
    Direction dir = Direction::TopToBottom;
    Shape     shape = Shape::Straight;

    auto line = std::make_unique<Line>(params.length, dir, shape, params.epilepsy);
    line->reset(width, height);
    activeLines.push_back(std::move(line));
}

void AppManager::updateAllLines() {
    for (auto& line : activeLines) {
        line->step();
    }
}

void AppManager::removeFinishedLines() {
    activeLines.erase(
        std::remove_if(activeLines.begin(), activeLines.end(),
            [](const std::unique_ptr<Line>& line) {
                return line->isFinished();
            }),
        activeLines.end()
    );
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
            updateAllLines();
            removeFinishedLines();
        }

        Sleep(10);
    }
}