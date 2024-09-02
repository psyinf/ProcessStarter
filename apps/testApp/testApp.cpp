#include <processLib/core/processLib.hpp>

#include <cli/CLI.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    CLI::App app{"Test Application"};

    uint16_t repetitions = 0;
    uint16_t milliseconds = 1000;
    app.add_option("-t, times", repetitions, "how many times to run, 0 means forever");
    app.add_option("-d, duration", milliseconds, "wait time per repetition in ms, defaults to 1000ms");

    CLI11_PARSE(app, argc, argv);

    using namespace std::chrono_literals;
    for (uint16_t i = 0; i < repetitions || repetitions == 0; ++i)
    {
        std::cout << "Repetition " << i << "\n";
        std::this_thread::sleep_for(milliseconds * 1ms);
    }

    return 0;
}