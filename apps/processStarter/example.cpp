#include "processInfo.hpp"
#include <processLib/core/processLib.hpp>
#include <vector>

void example()
{
    auto processInfoList = std::vector<processStarter::ProcessInfo>{
        {.name = "testApp",
         .path = "",
         .arguments = {{.option = "-t 100"}},
         .policies = processLib::Policies::defaultPolicies().withCreateNewConsole()} //,
        //    {.name = "testApp", .path = "", .arguments = {{.option = "-t 10000"}, {.option = "-d 10"}}}
    };

    auto processList = std::vector<processLib::Process>{};
    for (const auto& processInfo : processInfoList)
    {
        processLib::Process process(
            processInfo.name, processInfo.path, processInfo.getArgumentString(), processInfo.policies);
        process.start();
        processList.emplace_back(std::move(process));
    }
    for (auto& process : processList)
    {
        process.wait().get();
        process.stop();
    }
}