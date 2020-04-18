#include <chrono>
#include <iostream>
#include "application.h"

using namespace std::chrono;
using namespace snail;

static const std::vector<std::string> helpCommands { "-h", "-help", "--h", "--help", "?" };

static snail::application sApplication;

void showHelp();

int main(int argc, char* argv[])
{
    #if VERBOSITY_LEVEL >= 3
        std::filesystem::create_directory(std::string(DEBUG_OUTPUT_DIRECTORY));
    #endif

    snail::application application;
    snail::chai<double> script;

    script.getScript().add(chaiscript::fun([&application](mesh<double>* m, color c) { application.add_mesh(*m, c); }), "show");
    script.getScript().add(chaiscript::constructor<color (float, float, float, float)>(), "color");
    script.getScript().add(chaiscript::constructor<color (const color&)>(), "color");

    if (argc == 1 or (argc == 2 and std::find(helpCommands.begin(), helpCommands.end(), argv[1]) != helpCommands.end()))
    {
        showHelp();
        return 0;
    }

    // Get start time
    auto start = steady_clock::now();

    if (argc < 2)
    {
        snail::log(err) << "no script provided" << std::endl;
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        script.getScript().eval_file(argv[i]);
    }

    // Log time
    auto total = duration_cast<milliseconds>(steady_clock::now() - start).count();
    snail::log(info) << "Total time: " << total << "ms";
    snail::errorCounter::finish();

    return application.run();
}

void showHelp()
{
    std::cout << "blabla" << std::endl;
}
