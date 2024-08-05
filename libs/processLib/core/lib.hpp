#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include <future>

namespace detail {
class Process;

}

namespace processLib {

// a facade class that provides a simple interface to start, stop, restart, and get the status of a process
class Process
{
public:
    Process(const std::string& name, const Arguments& arguments);
    void                  start();
    ExitCode              stop();
    std::future<ExitCode> wait(
        std::chrono::system_clock::duration timeout = std::chrono::system_clock::duration::max());

private:
    ProcessConfig    _config;
    detail::Process* _process;
};

} // namespace processLib
