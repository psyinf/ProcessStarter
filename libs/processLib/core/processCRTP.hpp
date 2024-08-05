#pragma once

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>

namespace detail {

template <typename Derived>
class ProcessCRTP
{
public:
    ProcessCRTP(processLib::ProcessConfig config)
      : _config(config)
    {
    }

    void start() { static_cast<Derived*>(this)->startImpl(); }

    processLib::ExitCode stop() { return static_cast<Derived*>(this)->stopImpl(); }

    std::future<processLib::ExitCode> wait() { return static_cast<Derived*>(this)->wait(); }

    processLib::ProcessConfig _config;
};

#ifdef _WIN32
#include <windows.h>

LPSTR fromString(const std::string& str)
{
    if (str.empty()) { return nullptr; }
    else { return const_cast<LPSTR>(str.c_str()); }
}

class Process : public ProcessCRTP<Process>
{
public:
    using ProcessCRTP::ProcessCRTP;

    void startImpl()
    {
        STARTUPINFO startup_info = {0};

        // construct string from name and arguments
        std::string command_line = _config.name;
        auto        param_str = std::ranges::fold_left(
            _config.arguments | std::views::join_with(std::string{" "}), std::string{}, std::plus<>{});
        command_line += " " + param_str;
        CreateProcess(nullptr,
                      fromString(command_line),
                      nullptr,
                      nullptr,
                      TRUE,
                      0,
                      nullptr,
                      fromString(_config.path),
                      &startup_info,
                      &_process_info);
    }

    processLib::ExitCode stopImpl()
    {
        TerminateProcess(_process_info.hProcess, _exit_code);
        return _exit_code;
    }

    std::future<processLib::ExitCode> wait()
    {
        return std::async(std::launch::async, [this]() {
            WaitForSingleObject(_process_info.hProcess, INFINITE);

            GetExitCodeProcess(_process_info.hProcess, &_exit_code);
            return _exit_code;
        });
    }

private:
    PROCESS_INFORMATION  _process_info = {0};
    processLib::ExitCode _exit_code = 0;
};
#endif
} // namespace detail
