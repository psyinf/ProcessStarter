#pragma once
#include <optional>
#include <string>
#include <vector>

namespace processLib {
using Arguments = std::vector<std::string>;
using ExitCode = unsigned long;
using OptionalExitCode = std::optional<ExitCode>;

struct Policies
{
    bool stopOnWait = false; // if waiting for the process termination, stop the process if the timeout is reached
                             // before the process terminates
};

struct ProcessConfig
{
    std::string name;
    std::string path;
    Arguments   arguments;
    Policies    policies;
};

} // namespace processLib
