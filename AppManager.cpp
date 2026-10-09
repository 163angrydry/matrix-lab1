#include "AppManager.h"
#include "Line.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>

AppManager::AppManager(int argc, char* argv[]) : running(true) {
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
    if (argc != 4) {
        std::cerr << "Error: expected 3 arguments\n";
        showHelp();
        return false;
    }
    try {
        int speed = std::stoi(argv[1]);
        int length = std::stoi(argv[2]);
        std::string epi = argv[3];

        if (!validateSpeed(speed) || !validateLength(length) || !validateEpilepsy(epi))
            return false;

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
    std::cout << "Matrix - OOP Lab 1\n"
        << "Usage: matrix.exe [speed] [length] [epilepsy]\n"
        << "  speed    : 1..30 (chars per second)\n"
        << "  length   : 1..30 (chars)\n"
        << "  epilepsy : Y or N\n"
        << "Or run without arguments for dialog mode.\n";
}

void AppManager::runDialog() {
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

    params.speed = speed;
    params.length = length;
    params.epilepsy = (epi == "Y" || epi == "y");
}

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

void AppManager::mainLoop() {
    int width, height;
    ConsoleHelper::getConsoleSize(width, height);

    Direction dir = Direction::TopToBottom;
    Shape     shape = Shape::Straight;

    Line line(params.length, dir, shape, params.epilepsy);
    line.reset(width, height);

    DWORD stepMs = 1000 / params.speed;
    DWORD lastStep = GetTickCount();

    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        DWORD now = GetTickCount();
        if (now - lastStep >= stepMs) {
            lastStep = now;
            if (line.isFinished()) line.reset(width, height);
            line.step();
        }
        Sleep(10);
    }
}