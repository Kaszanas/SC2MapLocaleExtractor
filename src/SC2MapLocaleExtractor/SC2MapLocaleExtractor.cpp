// Standard
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <Windows.h>

// Third party:
#include "StormLib.h"
#include <nlohmann/json.hpp>

// Logging:
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

// My imports
#include "helpers.h"
#include "extractors.h"

int main(int argc, char** argv)
{

    helpers::initialize_logger();
    auto logger = spdlog::get("logger");

    logger->info("Initialized logger!");

    //if(argc < 3) {
    //    logger->warn("You have not provided required arguments!");
    //    logger->warn("Usage: {} <input_directory> <output_filepath_with_filename>.json", argv[0]);
    //    std::cout << "Press ENTER to continue...";
    //    std::getchar();
    //    return 1;
    //}

    std::string directoryString = "D:/Projects/GolangProjects/src/GoSC2Science/map_translator/MAPS";
    std::string outputFile = "output.json";

    //std::string directoryString = argv[1];
    logger->info("directoryString was specified as: {}", directoryString);
    //std::string outputFile = argv[2];
    logger->info("outputFile was specified as: {}", outputFile);

    std::optional<nlohmann::json> maybe_mapping = extractors::locale_extraction_pipeline(directoryString);
    if (maybe_mapping.has_value())
    {
        logger->info("Detected that maybe_mapping contains a value.");
        // Getting the final string value from json structure:
        nlohmann::json final_json_mapping = *maybe_mapping;
        std::string final_json_string = final_json_mapping.dump(4);
        logger->info("Dumped final_json_mapping into a string.");

        // Writing to drive at output location:
        std::ofstream out(outputFile);
        out << final_json_string;
        logger->info("Saved final_json_string to a file.");
    }
    else
    {
        logger->warn("Failed to obtain mapping, returning EXIT_FAILURE.");
        return EXIT_FAILURE;
    }

    std::cout << "Press ENTER to continue...";
    std::getchar();

    logger->info("Finished main(), returning EXIT_SUCCESS.");
    return EXIT_SUCCESS;
}