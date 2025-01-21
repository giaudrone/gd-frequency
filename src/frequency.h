#pragma once

#include <unordered_map>
#include <string>
#include <span>
#include <optional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using frequency_map = std::unordered_map<std::string, int64_t>;

void frequency(int const argc, char* argv[]);
