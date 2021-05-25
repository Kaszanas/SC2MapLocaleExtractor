#pragma once

#include <filesystem>
#include <optional>
#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"

namespace helpers
{
	// String functions
	std::wstring s2ws(const std::string& str);
	std::string ws2s(const std::wstring& wstr);

	// File system
	void directory_reader(std::vector<std::filesystem::path>& vector_to_populate, const std::string& input_directory, const std::string& get_extension);

	// String operations
	std::vector<std::string> split_string(const std::string& str, const std::string& delimiter);

	// JSON
	std::optional<nlohmann::json> generate_final_locale_mapping(const std::vector<nlohmann::json>& not_mapped_locales);

	// Logger
	void initialize_logger();
}