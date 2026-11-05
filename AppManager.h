#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Line.h"

struct AppParams {
    int  frequency;
    int  speed;
    int  length;
    bool epilepsy;
};

class AppManager {
public:
    AppManager(int argc, char* argv[]);
    void run();

private:
    AppParams params;
    bool running;

    std::vector<std::unique_ptr<Line>> activeLines;
    DWORD secondStart;
    std::vector<DWORD> spawnOffsets;
    size_t nextSpawnIndex;

    bool parseArgs(int argc, char* argv[]);
    void showHelp();
    void runDialog();

    static bool validateFrequency(int f);
    static bool validateSpeed(int s);
    static bool validateLength(int l);
    static bool validateEpilepsy(const std::string& s);

    void mainLoop();
    void regenerateSchedule();
    void spawnNewLine(int width, int height);
    void updateAllLines();
    void removeFinishedLines();
};
