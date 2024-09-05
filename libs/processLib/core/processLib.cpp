#include <processLib/core/processLib.hpp>
#include <processLib/core/processCRTP.hpp>

processLib::Process::Process(const std::string& name, const Arguments& arguments)

  : _config({name, "", arguments})
  , _process(new detail::Process(_config))
{
}

processLib::Process::Process(const std::string& name,
                             const std::string& path,
                             const Arguments&   arguments,
                             const Policies&    policies)

  : _config({name, path, arguments, policies})
  , _process(new detail::Process(_config))
{
}

processLib::Process::~Process()
{
    if (_process && isRunning()) { stop(); }
}

void processLib::Process::start()
{
    if (_process->isRunning())
    {
        switch (_config.policies.multiInstancePolicy)
        {
        case Policies::StartAlredyRunningPolicy::Restart:
            stop();
            break;
        case Policies::StartAlredyRunningPolicy::Throw:
            throw std::runtime_error("Process already running");
        case Policies::StartAlredyRunningPolicy::Ignore:
            return;
        }
    }
    _process->start();
}

processLib::OptionalExitCode processLib::Process::stop()
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
