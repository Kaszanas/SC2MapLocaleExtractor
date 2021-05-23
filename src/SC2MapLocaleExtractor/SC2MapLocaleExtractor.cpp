// Standard
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <Windows.h>

// Third party:
#include "StormLib.h"
#include <nlohmann/json.hpp>

// My imports
#include "helpers.h"
#include "extractors.h"

int main(int argc, char** argv)
{
    if(argc < 3) {
        std::cout << "You have not provided required arguments!" << "\n";
        std::cout << "Usage: "<< argv[0] <<" <input_directory> <output_filepath_with_filename>.json";
        return 1;
    }

    std::string directoryString = argv[1];
    std::string outputFile = argv[2];

    std::optional<nlohmann::json> maybe_mapping = locale_extraction_pipeline(directoryString);
    if (maybe_mapping.has_value())
    {
        // Getting the final string value from json structure:
        nlohmann::json final_json_mapping = *maybe_mapping;
        std::string final_json_string = final_json_mapping.dump(4);

        // Writing to drive at output location:
        std::ofstream out(outputFile);
        out << final_json_string;
    }
    else
    {
        return EXIT_FAILURE;
    }

    std::cout << "Press ENTER to continue...";
    std::getchar();

    return EXIT_SUCCESS;
}