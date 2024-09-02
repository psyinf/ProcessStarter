#pragma once

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

// nlohman json serialization
namespace serialization {
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(processStarter::Argument, option)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(processLib::Policies,
                                   stopOnWait,
                                   throwIfAlreadyStopped,
                                   startDetached,
                                   createNewConsole)

// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProcessInfo, name, path, arguments)
} // namespace serialization
} // namespace processStarter