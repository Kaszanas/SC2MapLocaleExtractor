## SC2MapLocaleExtractor

This repository contains a tool that allows to extract localization mapping from ```.SC2Map``` files into a ```.json``` output.

## Usage

This tool works only on Windows x86 or x64.
If You decide to use this tool and You have the ```.exe``` binary either built or downloaded then following command line arguments are required to be used for this tool to work.

```
SC2MapLocaleExtractor.exe <input_directory> <output_filepath_with_filename>.json
```

## Dependencies

1. [StormLib](https://github.com/Kaszanas/StormLib)
2. [nlohmann/json](https://github.com/nlohmann/json)

## Build

All required code is included in this repository.
Dependencies were placed in ```./src/third_party/```, if You wish to update them please refer to the current directory structure of the project.

### Initial setup

1. Clone the repository using the following command ```git clone --recurse-submodules https://github.com/Kaszanas/SC2MapLocaleExtractor.git```, this command will download all of the code including the forked version of [StormLib](https://github.com/Kaszanas/StormLib) and header only implementation of [nlohmann/json](https://github.com/nlohmann/json).
2. OPTIONAL (If You handle the dependencies by Yourself): Add StormLib either as a git submodule or directly clone it to ```./src/third_party/StormLib```.
3. Using Visual Studio open the solution ```./src/SC2MapLocaleExtractor.sln```
4. Change the solution configuration to "DebugUD" and build StormLib project (targeting x86) that is included.
5. Change the solution configuration to Debug and build the SC2MapLocaleExtractor project (targeting x86).

After completing the step above please follow the steps below.
If You use the included dependencies:

## Citation

If You use this tool please cite is as follows:

```
BiBTeX entry from Zenodo
```