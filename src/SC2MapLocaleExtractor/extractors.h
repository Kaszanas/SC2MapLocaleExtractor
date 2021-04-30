#pragma once

#include <filesystem>
#include <optional>
#include <nlohmann/json.hpp>

std::optional<nlohmann::json> locale_extraction_pipeline(const std::string& filepath);