#pragma once
#include <string>
#include <vector>

namespace processLib {
using Arguments = std::vector<std::string>;
using ExitCode = unsigned long;

struct ProcessConfig
{
    std::string name;
    std::string path;
    Arguments   arguments;
};

} // namespace processLib