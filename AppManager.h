#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Figure.h"
#include "Line.h"
#include "Explosion.h"

struct AppParams {
    int  frequency;
    int  speed;
    int  length;
    bool epilepsy;
    int  explosionProbability;
    int  minExplosionRadius;
    int  maxExplosionRadius;
};

class AppManager {
public:
    AppManager(int argc, char* argv[]);
    void run();

private:
    AppParams params;
    bool running;

    std::vector<std::unique_ptr<Figure>> activeFigures;
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
    static bool validateExplosionProbability(int p);
    static bool validateMinRadius(int r);
    static bool validateMaxRadius(int rMin, int rMax);

    void mainLoop();
    void regenerateSchedule();
    void spawnNewLine(int width, int height);
    void updateAllFigures();
    void removeFinishedFigures();
};