## SC2MapLocaleExtractor

This repository contains a tool that allows to extract localization mapping from ```.SC2Map``` files into a ```.json``` output.

## Usage

This tool works only on Windows x64 or x86.
If You decide to use this tool and You have the ```.exe``` binary either built or downloaded then following command line arguments are required to be used for this tool to work.

```
SC2MapLocaleExtractor.exe <input_directory> <output_filepath_with_filename>.json
```

## Build

1. Add StormLib either as a git submodule or directly clone it to ```./src/third_party/StormLib```.
2. Build StormLib as ReleaseUD.
3. Build the final solution in Release configuration targeting x86.

## Citation

If You use this tool please cite is as follows:

```
BiBTeX entry from Zenodo
```