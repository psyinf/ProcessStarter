#pragma once

#include <string>

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
    std::vector<Argument> arguments;

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

} // namespace processStarter