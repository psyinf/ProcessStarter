#include <processLib/core/processLib.hpp>
#include "processInfoLoader.hpp"
#include <processInfo.hpp>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <CLI/CLI.hpp>

void test()
{
    processStarter::ProcessInfoList processInfoList{.processInfoList{
        {.name = "testApp",
         .path = "",
         .arguments = {{.option = "-t 100"}},
         .policies = processLib::Policies::defaultPolicies().withCreateNewConsole()} //,
        //    {.name = "testApp", .path = "", .arguments = {{.option = "-t 10000"}, {.option = "-d 10"}}}
    }};
    std::ofstream                   os("processList.json");
    nlohmann::json                  processListJson = processInfoList;
    os << processListJson.dump(4) << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    test();
    CLI::App    app{"Process Starter"};
    std::string processList = "processList.yaml";
    app.add_option("-p, --processList", processList, "path to process list file");

    CLI11_PARSE(app, argc, argv);
    // load process list
    auto processInfo = processStarter::loadFromFile(processList);
    return 0;
}