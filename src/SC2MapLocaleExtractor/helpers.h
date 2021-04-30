#pragma once

#include <filesystem>
#include <optional>
#include <nlohmann/json.hpp>

std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);
void directory_reader(std::vector<std::filesystem::path>& vector_to_populate, const std::string& input_directory, const std::string& get_extension);
std::vector<std::string> split_string(const std::string& str, const std::string& delimiter);
std::optional<nlohmann::json> generate_final_locale_mapping(const std::vector<nlohmann::json>& not_mapped_locales);