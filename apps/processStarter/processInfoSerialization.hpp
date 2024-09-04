#pragma once
#include <processInfo.hpp>
#include <processLib/core/config.hpp>
#include <nlohmann/json.hpp>

namespace processLib {
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(processLib::Policies,
                                                stopOnWait,
                                                throwIfAlreadyStopped,
                                                startDetached,
                                                createNewConsole)
} // namespace processLib

namespace processStarter {

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Argument, option)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ProcessInfo, name, path, arguments, policies)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(ProcessInfoList, processInfoList)

} // namespace processStarter