#pragma once
#include <optional>
#include <stdexcept>
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
    bool throwIfAlreadyStopped = false; // attempting to stop and non-running job will throw an exception
    bool startDetached = false;         // start the process detached from the current process
    bool createNewConsole = false;      // create a new console for the process, not compatible with startDetached

    static Policies defaultPolicies() { return Policies{}; }

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

    Policies& withStartDetached(bool startDetached = true)
    {
        if (startDetached && createNewConsole)
        {
            throw std::invalid_argument("Cannot start detached and create a new console");
        }
        this->startDetached = startDetached;
        return *this;
    }

    Policies& withCreateNewConsole(bool createNewConsole = true)
    {
        if (startDetached && createNewConsole)
        {
            throw std::invalid_argument("Cannot start detached and create a new console");
        }
        this->createNewConsole = createNewConsole;
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
