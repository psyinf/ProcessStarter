#pragma once

#include <processLib/core/processLib.hpp>
#include <string>
#include <vector>
#include <ranges>

#include <nlohmann/json.hpp>

namespace processStarter {

struct Argument
{
    std::string option;

    std::string getArgumentString() const { return option; }
};

struct ProcessInfo
{
    std::string           name;
    std::string           path;
    std::vector<Argument> arguments{};
    Argument              argument;
    processLib::Policies  policies;

    std::vector<std::string> getArgumentString() const
    {
        std::vector<std::string> argumentVec;
        for (const auto& argument : arguments)
        {
            argumentVec.push_back(argument.getArgumentString());
        }
        return argumentVec;
    }
};

struct ProcessInfoList
{
    std::vector<ProcessInfo> processInfoList;
};

} // namespace processStarter