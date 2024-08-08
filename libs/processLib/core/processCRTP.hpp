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

    processLib::OptionalExitCode stop() { return static_cast<Derived*>(this)->stopImpl(); }

    std::future<processLib::ExitCode> wait(std::chrono::system_clock::duration timeout)
    {
        return static_cast<Derived*>(this)->wait(timeout);
    }

    bool isRunning() const { return static_cast<const Derived*>(this)->isRunningImpl(); }

    processLib::ProcessConfig _config;
};

#ifdef _WIN32
#include <windows.h>

LPSTR fromString(const std::string& str)
{
    if (str.empty()) { return nullptr; }
    else { return const_cast<LPSTR>(str.c_str()); }
}

std::string lastErrorToString(DWORD error_code)
{
    LPSTR buffer = nullptr;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
                      FORMAT_MESSAGE_MAX_WIDTH_MASK,
                  nullptr,
                  error_code,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPSTR)&buffer,
                  0,
                  nullptr);
    std::string message(buffer);
    LocalFree(buffer);
    return message;
}

class Process : public ProcessCRTP<Process>
{
public:
    using ProcessCRTP::ProcessCRTP;

    ~Process() = default;

    void startImpl()
    {
        STARTUPINFO startup_info = {0};

        // construct string from name and arguments
        std::string command_line = _config.name;
        auto        param_str = std::ranges::fold_left(
            _config.arguments | std::views::join_with(std::string{" "}), std::string{}, std::plus<>{});
        command_line += " " + param_str;
        auto result = CreateProcess(nullptr,
                                    fromString(command_line),
                                    nullptr,
                                    nullptr,
                                    TRUE,
                                    0,
                                    nullptr,
                                    fromString(_config.path),
                                    &startup_info,
                                    &_process_info);

        if (result == 0) { throw std::runtime_error("Create process failed: " + lastErrorToString(GetLastError())); }
    }

    processLib::OptionalExitCode stopImpl()
    {
        processLib::ExitCode exit_code{};
        auto                 ret = TerminateProcess(_process_info.hProcess, exit_code);
        if (ret == 0)
        {
            if (_config.policies.throwIfAlreadyStopped)
            {
                throw std::runtime_error("Stop process failed: " + lastErrorToString(GetLastError()));
            }
        }
        else { _exit_code = exit_code; }

        return _exit_code;
    }

    std::future<processLib::OptionalExitCode> wait(std::chrono::system_clock::duration timeout)
    {
        return std::async(std::launch::async, [this, timeout]() {
            auto wait_result = WaitForSingleObject(
                _process_info.hProcess, std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count());

            switch (wait_result)
            {
            case WAIT_OBJECT_0: {
                processLib::ExitCode exit_code{};
                GetExitCodeProcess(_process_info.hProcess, &exit_code);
                CloseHandle(_process_info.hProcess);
                CloseHandle(_process_info.hThread);
                _exit_code = exit_code;
            }
            break;
            case WAIT_TIMEOUT: {
                if (_config.policies.stopOnWait) { stopImpl(); };
            }
            break;
            case WAIT_FAILED:
                // TODO: policy to control if we throw here or return an error code
                break;

            default:
                break;
            }
            return _exit_code;
        });
    }

    std::optional<processLib::ExitCode> getExitCode() const { return _exit_code; }

    bool isRunningImpl() const
    {
        DWORD exit_code;
        GetExitCodeProcess(_process_info.hProcess, &exit_code);
        return exit_code == STILL_ACTIVE;
    }

private:
    PROCESS_INFORMATION _process_info = {0};

    processLib::OptionalExitCode _exit_code = std::nullopt;
};
#endif
} // namespace detail
