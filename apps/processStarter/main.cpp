#include <processLib/core/lib.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    processLib::Process process(std::string("ping"), processLib::Arguments{"localhost"});
    process.start();
    process.wait().get();

    return 0;
}