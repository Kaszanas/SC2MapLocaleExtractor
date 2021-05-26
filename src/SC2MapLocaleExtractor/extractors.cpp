// Standard
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <Windows.h>

// Third party:
#include "StormLib.h"
#include <nlohmann/json.hpp>

// My imports
#include "helpers.h"

namespace extractors
{

    // Function that extracts one locale file information from MPQ Archive:
    std::optional<std::string> extract_locale_from_mpq(HANDLE mpqArchive, const SFILE_FIND_DATA& file_data)
    {
        auto logger = spdlog::get("logger");
        logger->info("Entered extract_locale_from_mpq()");
        HANDLE openedLocaleFile;
        bool result = SFileOpenFileEx(mpqArchive, file_data.cFileName, SFILE_OPEN_FROM_MPQ, &openedLocaleFile);
        if (!result)
        {
            logger->warn("Couldn't open the file that is within MPQ Archive, error: {}", GetLastError());
            return std::nullopt;
        }

        std::vector<char> localeFileBuffer(file_data.dwFileSize, '\0');

        DWORD bytesRead;
        result = SFileReadFile(openedLocaleFile, localeFileBuffer.data(), localeFileBuffer.size() * sizeof(decltype(localeFileBuffer)::value_type), &bytesRead, nullptr);
        if (!result) {
            logger->warn("Couldn't read the file that was found within MPQ Archive, error: {}", GetLastError());
            return std::nullopt;
        }

        SFileCloseFile(openedLocaleFile);
        logger->info("Closed MPQ file!");
        std::string locale(localeFileBuffer.begin(), localeFileBuffer.end());
        logger->info("Finished extract_locale_from_mpq() returning.");
        return std::make_optional(locale);
    }


    // Function extracting localized map name:
    std::optional<std::string> extract_map_name_string(std::string game_description)
    {
        auto logger = spdlog::get("logger");
        logger->info("Entered extract_map_name_string()");
        // Splitting the string into lines:
        std::vector<std::string> newline_split = helpers::split_string(game_description, "\r\n");

        auto it = std::find_if(newline_split.begin(), newline_split.end(), [](const std::string& parsed_line) {
            return parsed_line.find("DocInfo/Name") != std::string::npos;
        });

        if (it != newline_split.end()) {
            const std::string& parsed_line = *it;
            std::vector<std::string> map_name_line_split = helpers::split_string(parsed_line, "=");
            std::string map_name = map_name_line_split[1];
            if (map_name.find(" /// "))
            {
                std::vector<std::string> map_name_split_comment = helpers::split_string(map_name, " /// ");
                map_name = map_name_split_comment[0];
            }

            if (map_name.empty())
            {
                // TODO: Empty map check doesn't allow to find what is the currently processed map and doesn't allow for sufficient debugging later on.
                logger->warn("Detected empty map_name returning.");
                return std::nullopt;
            }
            logger->info("Finished extract_map_name_string(), returning.");
            return std::make_optional(map_name);
        }
        logger->warn("Detected empty map name returning.");
        return std::nullopt;
    }


    // Function finding locale name from MPQ archive internal path:
    // "koKR.SC2Data\\LocalizedData\\GameStrings.txt" -> "koKR"
    std::string locale_region_extractor(const std::string& region_name)
    {
        auto logger = spdlog::get("logger");
        logger->info("Entered locale_region_extractor()");
        std::string dot_delimiter = ".";
        std::string locale_region = region_name.substr(0, region_name.find(dot_delimiter));
        logger->info("Calculated locale_region = {}", locale_region);
        return locale_region;
    }

    // Function extracting locale information from a single MPQ ".SC2Map" file
    // sample output: {"koKR": "koreanMapName", "enUS": "englishMapName"}
    std::optional<nlohmann::json> locale_extractor(const std::filesystem::path& filepath)
    {
        auto logger = spdlog::get("logger");
        logger->info("Entered locale_extractor()");
        // Opening MPQ Archive:
        HANDLE MPQArchive;
        if (!SFileOpenArchive(filepath.c_str(), 0, 0, &MPQArchive))
        {
            logger->warn("Couldn't open MPQ archive, error: {}", GetLastError());
            return std::nullopt;
        }

        // Looking for GameStrings.txt files containing the translated map names:
        SFILE_FIND_DATA foundLocaleFileData;

        HANDLE searchHandle = SFileFindFirstFile(MPQArchive, "*GameStrings.txt", &foundLocaleFileData, nullptr);
        if (searchHandle == NULL)
        {
            logger->warn("Couldn't find file in MPQ archive, error: {}", GetLastError());
            return std::nullopt;
        }

        // Looping over all of the files that are in MPQ archive that contain locale information:
        nlohmann::json myMapping;
        do
        {
            // Calling helper method to 
            auto maybe_locale = extract_locale_from_mpq(MPQArchive, foundLocaleFileData);
            // Checking if data was extracted:
            if (!maybe_locale.has_value()) {
                logger->warn("Couldn't extract locale from MPQ");
                continue;
            }

            // Obtaining region code:
            std::string region_name = foundLocaleFileData.cFileName;
            std::string locale_region = locale_region_extractor(region_name);

            // Extracting map name:
            std::optional<std::string> maybe_map_name_string = extract_map_name_string(*maybe_locale);
            if (!maybe_map_name_string.has_value())
            {
                logger->warn("Detected empty map_name_string in filepath: {}. This happened while parsing region: {}. Skipping!", filepath.string(), locale_region);
                continue;
            }
            std::string map_name_string = *maybe_map_name_string;

            myMapping[locale_region] = map_name_string;

        } while (SFileFindNextFile(searchHandle, &foundLocaleFileData));

        // Freeing up resource:
        SFileCloseFile(searchHandle);
        logger->info("Closed searched file.");
        SFileCloseArchive(MPQArchive);
        logger->info("Closed MPQ archive.");

        return std::make_optional(myMapping);
    }


    // Function that wraps all of the smaller processing logic and creates a final mapping for a whole directory of MPQ ".SC2Map" files.
    // Sample output: {"koreanMapName": "englishMapName", "polishMapName": "englishMapName"}
    std::optional<nlohmann::json> locale_extraction_pipeline(const std::string& filepath)
    {
        auto logger = spdlog::get("logger");
        logger->info("Entered locale_extraction_pipeline()");
        // Initializing empty mapping container:
        std::vector<nlohmann::json> mapping_container;
        std::vector<std::filesystem::path> detected_files;
        logger->info("Initialized variables mapping_container and detected_files.");

        // Reading directory contents from filepath:
        helpers::directory_reader(detected_files, filepath, ".SC2Map");
        if (detected_files.empty())
        {
            logger->warn("directory_reader failed to populated detected_files! Returning std::nullopt");
            return std::nullopt;
        }

        // Extracting locale mapping for every file:
        for (const std::filesystem::path& file : detected_files)
        {
            // Extracting the mapping e.g. {"enUS": "mapName"}
            std::optional<nlohmann::json> maybe_extracted_mapping = locale_extractor(file);
            if (maybe_extracted_mapping.has_value())
            {
                mapping_container.push_back(*maybe_extracted_mapping);
            }
            else
            {
                logger->warn("Couldn't extract locale information from a file: {}", file.string());
                continue;
            }
        }

        // Transforming to final mapping that will be written to drive:
        std::optional<nlohmann::json> maybe_final_mapping = helpers::generate_final_locale_mapping(mapping_container);
        if (maybe_final_mapping.has_value())
        {
            nlohmann::json final_mapping = *maybe_final_mapping;
            return std::make_optional(final_mapping);
        }

        logger->warn("Failed to generate final locale mapping! Returning std::nullopt");
        return std::nullopt;
    }

}