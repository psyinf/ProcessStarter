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
    bool throwIfAlreadyStopped = false; // attempting to stop and non-runnign job will throw an exception

    // monadic interface
    Policies& withStopOnWait(bool stopOnWait = true)
    {
        this->stopOnWait = stopOnWait;
        return *this;
    }

    Policies& withThrowIfAlreadyStopped(bool throwIfAlreadyStopped = true)
    {
        this->throwIfAlreadyStopped = throwIfAlreadyStopped;
        return *this;
    }
};

struct ProcessConfig
{
    std::string name;
    std::string path;
    Arguments   arguments;
    Policies    policies;
};

} // namespace processLib
