# ot3_firmware

This repository holds the firmware for the OT-3 and all of its peripheral systems. **Note** that the cmake directory is
actually a subtree of cmake-utils. Do not make changes to the `cmake-utils` directory in the ot3_firmware repository.
Instead, navigate to the [CMake Utils Repository](https://github.com/Opentrons/cmake-utils).

## Structure of directories

Aside from the common directory, each repository should contain a `firmware`, `include`, `src`, `tests` folder.

1. `firmware` should hold code that will be controlling the peripheral system.
2. `include` should hold the majority of the header files you will need. This folder should include another subset of
   directories labeled `firmware`, `src` and `tests` for import cleaness.
3. `src` should include any libraries, 3D party or otherwise, that are required for `firmware` to control the peripheral
   system.
4. `tests` should include tests for the firmware folder.

## Working with CMake

This project uses cmake as a build and configuration system. It uses [cmake presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) to ease remembering commands. It requires at least CMake 3.20 (to support build presets) to run.

Using a preset is required for configuration to prevent mistakes that can create directories all over the place.

To list available configuration presets, run `cmake --list-presets`. You can then run `cmake --preset=selected-preset .` to configure, which will create an appropriate binary directory.

Building has presets now too. You can run `cmake --build --list-presets` to show available build presets. They're linked to configuration presets, and you can run them with `cmake --build --preset=selected preset` to build their default target in the appropriate binary directory.

When building, even with a preset, you can set any target you want with `--target`. To prevent mistakenly using a target from the wrong cross configuration, executables all have their own presets, which depend on the correct configuration. When building a target, always try and use the preset that matches that target. For instance, if you're trying to debug a gantry board, run `cmake --build --preset=gantry --target gantry-debug`.


## Setup

To setup this directory to run on an STM32G4 system board (gantry and head), you should run:

1. `cmake --preset=cross .`
2. `cmake --build --preset=gantry --target <TARGET>` or `cmake --build --preset=head --target <TARGET>`

To setup this directory to run on an STML5 nucleo board (pipettes), you should run:

1. `cmake --preset=cross-pipettes .`
2. `cmake --build --preset=pipettes --target <TARGET>`

To setup this directory to run tests, you should run:

1. `cmake --preset=host .`
   If you are on OSX, you almost certainly want to force cmake to select gcc as the compiler used for building tests,
   because the version of clang built into osx is weird. We don't really want to always specify the compiler to use in
   tests, so forcing gcc is a separate cmake config preset, and it requires installing gcc 10:

   `brew install gcc@10`
   `cmake --preset=host-gcc10 .`
2. `cmake --build ./build-host --target build-and-test`, which will run all of the tests available in each periphery.
3. or, `cmake --build ./build-host --target <TARGET>-build-and-test`

#### Gantry Subsystem

The default axis type for the gantry target is X. You can build the Y gantry by running:

1. `cmake --preset=cross -DGANTRY_AXIS_TYPE=gantry_y .`
2. `cmake --build --preset=gantry`

### Cross-compiling vs Host-compiling

As you saw above, two different presets were used to run tests vs running on a physical device. Cross-compiling will
generally be used when the code needs to be ported to other devices (in this case the microcontroller) while
host-compiling is used when you want to run things like tests. For more information check out this
helpful [article](https://landley.net/writing/docs/cross-compiling.html).

## Run

Connect to an STM32 nucleo board and run either:

1. `make <TARGET>-debug` inside the build-cross folder
2. or, `cmake --build ./build-cross --target <TARGET>-debug` from the top-level folder.

### Debug

If you run into trouble starting a gcc connection to your nucleo board, you should (while connected to the board):

Start openocd:

1. Navigate to `stm32-tools/openocd/Darwin`
2. `./bin  scripts/board/st_nucleo_f3.cfg` (or `scripts/board/st_nucleo_g4.cfg` for gantry/head)

Start gcc:

1. `./stm32-tools/gcc-arm-embedded/Darwin/bin/arm-none-eabi-gdb-py <TARGET>`
2. `target extended-remote :3333`

And then run all the lines before the error to see what is causing the problem.

## Common

The common directory will hold code that should be shared amongst all peripheral systems.

## Pipettes

This is the top-level directory for running the pipette peripheral system. Eventually, we will most likely have separate
directories for different applications on the pipettes.
