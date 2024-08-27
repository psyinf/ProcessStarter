#include <processLib/core/processLib.hpp>
#include <ProcessStarter.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    auto processInfoList = std::vector<processStarter::ProcessInfo>{

        {.name = "ping", .path = "", .arguments = {{.option = "localhost"}}},
        {.name = "ping", .path = "", .arguments = {{.option = "localhost"}, {.option = "-t"}}}};

    auto processList = std::vector<processLib::Process>{};
    for (const auto& processInfo : processInfoList)
    {
        processLib::Process process(processInfo.name, processInfo.path, processInfo.getArgumentString());
        process.start();
        processList.emplace_back(std::move(process));
    }
    for (auto& process : processList)
    {
        process.wait().get();
        process.stop();
    }

    //     using namespace std::chrono_literals;
    //     processLib::Process process(std::string("ping"), processLib::Arguments{"localhost", "-t"});
    //     process.start();
    //     process.wait().get();

    return 0;
}