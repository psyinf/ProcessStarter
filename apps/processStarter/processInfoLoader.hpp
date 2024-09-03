#pragma once
#include "processInfoSerialization.hpp"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include "yaml2json.hpp"

namespace processStarter {

inline auto loadJson(const std::filesystem::path& file)
{
    std::ifstream is(file.string(), std::ios_base::binary);
    return nlohmann::json::parse(is);
}

inline auto loadYaml(const std::filesystem::path& file)
{
    return convertYamlFile(file.string());
}

inline ProcessInfoList loadFromFile(const std::filesystem::path& file)
{
    if (file.extension() == ".json") { return loadJson(file); }
    else if (file.extension() == ".yaml") { return loadYaml(file); }
    else { throw std::runtime_error("unsupported file extension"); }
}

} // namespace processStarter