#include <processLib/core/lib.hpp>
#include <processLib/core/processCRTP.hpp>

processLib::Process::Process(const std::string& name, const Arguments& arguments)

  : _config({name, "", arguments})
  , _process(new detail::Process(_config))
{
}

processLib::Process::~Process()
{
    if (!getExitCode().has_value()) { stop(); }
}

void processLib::Process::start()
{
    _process->start();
}

processLib::ExitCode processLib::Process::stop()
{
    return _process->stop();
}

std::future<processLib::OptionalExitCode> processLib::Process::wait(std::chrono::system_clock::duration timeout)
{
    return _process->wait(timeout);
}

std::optional<processLib::ExitCode> processLib::Process::getExitCode() const
{
    return _process->getExitCode();
}

bool processLib::Process::isRunning() const
{
    return _process->isRunning();
}
