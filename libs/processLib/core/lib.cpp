#include <processLib/core/lib.hpp>
#include <processLib/core/processCRTP.hpp>

processLib::Process::Process(const std::string& name, const Arguments& arguments)

  : _config({name, "", arguments})
  , _process(new detail::Process(_config))
{
}

void processLib::Process::start()
{
    _process->start();
}

processLib::ExitCode processLib::Process::stop()
{
    return _process->stop();
}

std::future<processLib::ExitCode> processLib::Process::wait()
{
    return _process->wait();
}
