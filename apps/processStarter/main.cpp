#include <processLib/core/lib.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    using namespace std::chrono_literals;
    processLib::Process process(std::string("ping"), processLib::Arguments{"localhost", "-t"});
    process.start();
    process.wait().get();

    return 0;
}