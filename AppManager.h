#pragma once
#include <string>

struct AppParams {
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

    bool parseArgs(int argc, char* argv[]);
    void showHelp();
    void runDialog();

    static bool validateSpeed(int s);
    static bool validateLength(int l);
    static bool validateEpilepsy(const std::string& s);

    void mainLoop();
};