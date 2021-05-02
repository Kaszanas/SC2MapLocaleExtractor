## SC2MapLocaleExtractor

This repository contains a tool that allows to extract localization mapping from a directory containing ```.SC2Map``` files into a ```.json``` output.
The extraction is mapping foreign map names into English map names as follows: ```{"foreignMapName": "englishMapName"}```

### Sample output data

```
{
    "16 Bits LE": "16-Bit LE",
    "16 bits EC": "16-Bit LE",
    "16 bits EE": "16-Bit LE",
    "16 bits EJ": "16-Bit LE",
    "16 bitów ER": "16-Bit LE",
    "16 бит РВ": "16-Bit LE",
    "16位-天梯版": "16-Bit LE",
    "16位元 - 天梯版": "16-Bit LE",
    "16비트 - 래더": "16-Bit LE",
    "Akropol ER": "Acropolis LE",
    "Akropolis LE": "Acropolis LE",
    "Acropoli LE": "Acropolis LE",
    "Acropolis EC": "Acropolis LE",
    "Acrópole LE": "Acropolis LE",
    "Acrópolis EE": "Acropolis LE",
    "Acrópolis EJ": "Acropolis LE",
}
```

## Usage

This tool works only on Windows x86 or x64.
If You decide to use this tool and You have the ```.exe``` binary either built or downloaded then following command line arguments are required to be used for this tool to work.

```
SC2MapLocaleExtractor.exe <input_directory> <output_filepath_with_filename>.json
```

## Requirements

1. Visual Studio 2019 - Desktop development with C++ workload
2. Git

## Dependencies

1. [StormLib](https://github.com/Kaszanas/StormLib)
2. [nlohmann/json](https://github.com/nlohmann/json)

## Build

All required code is included in this repository.
Dependencies were placed in ```./src/third_party/```, if You wish to update them please refer Notes section below.

1. Open a command prompt that recognizes "msbuild" command, we recommend: "x86 Native Tools Command Prompt for VS 2019"
2. Clone the repository using the following command:
```
git clone --recurse-submodules https://github.com/Kaszanas/SC2MapLocaleExtractor.git && cd SC2MapLocaleExtractor/src
```
this command will download all of the code including the forked version of [StormLib](https://github.com/Kaszanas/StormLib) and header only implementation of [nlohmann/json](https://github.com/nlohmann/json).

3. Build StormLib by running the following command: 
```
cd third_party/StormLib && msbuild StormLib_vs19.vcxproj /p:Configuration=DebugUD && cd ../..
```

4. Build SC2MapLocaleExtractor by running the following command:
```
cd SC2MapLocaleExtractor && msbuild SC2MapLocaleExtractor.vcxproj /p:Configuration=Debug
```

5. Change the directory to where the ```SC2MapLocaleExtractor.exe``` was build by running the following command:
```
cd Debug
```
6. Use the tool in accordance with Usage section.

## Notes

If You wish to use the newest version of [StormLib](https://github.com/Kaszanas/StormLib) that was not forked, Add StormLib either as a git submodule or directly clone it to ```./src/third_party/```.

## Cite us

If You use this tool please cite is as follows:

```
BiBTeX entry from Zenodo
```

## Contributions

- Andrzej Białecki - conceptualization, programming, documentation,
- Piotr Białecki - Technical oversight (consulting, code review).
