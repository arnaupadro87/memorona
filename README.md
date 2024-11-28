# Environment

Tested in a Ubuntu 24.04. Should work in any environment with CMake and Qt installed maybe with some minor variations in the build process.

# Pre build steps

The project does not include images that will be used for the game. You can add them at `images/Cards` folder and then add each of the files in the `images.qrc` file to be included in the compilation of the project.

# Build instructions

From inside the project folder execute the following commands:

```
cmake . -B ../build-memora-release
cmake --build ../build-memora-release/ --parallel
```

It is also possible to use any graphical IDE with CMake support to make the compilation.

# Execute

From inside the project folder execute the following commands:

```
cd ../build-memora-release
./OnaMemorona
```
