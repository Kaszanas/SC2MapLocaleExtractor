//Standard 
#include <filesystem>
#include <iostream>
#include <optional>
#include <vector>
#include <Windows.h>

// Third party
#include <nlohmann/json.hpp>

// My imports
#include "helpers.h"


// string to wstring helper function:
std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}


// wstring to string helper function:
std::string ws2s(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}


// Function that reads and lists a directory into a supplied vector:
void directory_reader(std::vector<std::filesystem::path>& vector_to_populate, const std::string& input_directory, const std::string& get_extension)
{
    // Iterating over files that were detected in a directory:
    for (const auto& file : std::filesystem::directory_iterator(input_directory))
    {
        // Checking if file extension matches the target:
        if (file.path().filename().extension().string() == get_extension)
        {
            // Adding relevant filepaths to a vector:
            vector_to_populate.push_back(file.path());
        }
    }

}


// Function that splits a string and returns a vector of strings:
std::vector<std::string> split_string(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

std::optional<nlohmann::json> generate_final_locale_mapping(const std::vector<nlohmann::json>& not_mapped_locales)
{
    // Initializing function variables:
    nlohmann::json final_mapping;

    for (nlohmann::json map_locale : not_mapped_locales)
    {
        // Check if enUS is a key in the mapping
        if (map_locale.contains("enUS"))
        {
            std::string en_map_name = map_locale["enUS"];
            // Iterating over all of the values and do {"foreignName": "englishName"}
            for (auto& foreign_map_mapping : map_locale.items())
            {
                // Mapping foreign name to english name:
                std::string foreign_map_name = foreign_map_mapping.value();

                if (foreign_map_name != en_map_name)
                {
                    if (foreign_map_name == "")
                    {
                        std::cout << "Detected empty string as a foreign_map_name, skipping. This happened when reading: " << en_map_name << "\n";
                        continue;
                    }
                    final_mapping.emplace(foreign_map_name, en_map_name);
                }
            }
        }
        else
        {
            std::cout << "One of the map locales did not contain 'enUS' version, returning" << "\n";
            return std::nullopt;
        }
    }
    return std::make_optional(final_mapping);
}