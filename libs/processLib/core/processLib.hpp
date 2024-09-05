#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include <future>

namespace detail {
class Process;

} // namespace detail

namespace processLib {

// a facade class that provides a simple interface to start, stop, restart, and get the status of a process
class Process
{
public:
    Process(const std::string& name, const Arguments& arguments);

    Process(const std::string& name,
            const std::string& path,
            const Arguments&   arguments,
            const Policies&    policies = Policies());

    ~Process();

    // move constructor
    Process(Process&&) noexcept = default;
    // move assignment
    Process& operator=(Process&&) noexcept = default;
    // dissallow copy
    Process(const Process&) = delete;
    Process& operator=(const Process&) = delete;

    /**
     *  @brief Start the process
     */
    void start();
    /**
     * @brief stop the process. Depending on the configuration an exception may be thrown if the process is not running
     * @return the optional exit code of the process.
     */
    OptionalExitCode stop();
    /**
     * @brief wait for the process to finish
     * @param timeout the maximum time to wait for the process to finish, default is infinite
     * @return a future yielding an optional exit code. Depending on the configuration, the process may be stopped or
     * detached
     */
    std::future<OptionalExitCode> wait(
        std::chrono::system_clock::duration timeout = std::chrono::system_clock::duration::max());
    /**
     * @brief get the exit code of the process
     * @return the exit code of the process
     */
    OptionalExitCode getExitCode() const;

    /**
     * @brief get the configuration of the process
     * @return the configuration of the process
     */
    [[nodiscard]] auto getConfig() const { return _config; }

    /**
     * @brief get the name of the process
     * @return the name of the process
     */
    [[nodiscard]] auto getName() const { return _config.name; }

    /**
     * @brief get the arguments of the process
     * @return the arguments of the process
     */
    [[nodiscard]] auto getArguments() const { return _config.arguments; }

    /**
     * @brief get the path of the process
     * @return the path of the process
     */
    [[nodiscard]] auto getPath() const { return _config.path; }

    /**
     * @brief return running state
     * @return true if the process is running, false otherwise
     */
    bool isRunning() const;

private:
    ProcessConfig                    _config;
    std::shared_ptr<detail::Process> _process; // TODO: replace with unique_ptr, but incomplete type is not yet solved
};

} // namespace processLib
