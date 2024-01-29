# Smol Game Engine

This smol game engine is a students project with the purpose of investigate and improve my own coding skills. It is based only on OpenGL, SDL2 and the only platforma available to use it is Windows.

This project uses a little quick-start project template for C++ projects which utilise a Core/App project architecture. There are two included projects - one called _Core_, and one called _App_. [Premake](https://github.com/premake/premake-core) is used to generate project files.

Core builds into a static library and is meant to contain common code intended for use in multiple applications. App builds into an executable and links the Core static library, as well as provides an include path to Core's code.

The `Scripts/` directory contains build scripts for Windows and Linux, and the `Vendor/` directory contains Premake binaries (currently version `5.0-beta2`).

## Setup
1. Clone or download this repository
2. `App/` and `Core/` are the two main projects
3. Open the `Scripts/` directory and run the `Setup` script to generate projects files. It is set to Visual Studio 2019 for Windows but can support vs2022 by modifying the `.bat`

Note that no macOS or linux setup script is currently provided.

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)
