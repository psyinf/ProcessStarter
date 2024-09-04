#include <processLib/core/processLib.hpp>
#include <iostream>

#ifdef _WIN32
auto arguments = processLib::Arguments{"localhost", "-t"};
#else
auto arguments = processLib::Arguments{"localhost"};
#endif

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    using namespace std::chrono_literals;
    // start indefinite ping process

    processLib::Process process(std::string("ping"), arguments);
    process.start();
    // abort after 10sec
    auto result = process.wait(5s).get();
    process.stop();
    std::cout << "Process " << process.getName() << "terminated with " << process.getExitCode().value_or(-255) << "\n ";
    return 0;
}