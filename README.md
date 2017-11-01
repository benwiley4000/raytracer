# raytracer

Load a 3D scene file and render a 2D image using ray tracing.

Works on macOS and Linux. Should work for Windows, but not recently tested.

## Features

TBA

## Building

See [Building and running the project](docs/building.md).

## Acknowledgements

### Dependencies

Beyond the C++ standard library this application relies on:
* GLM
* STB (particularly the stb_image.h header library)

(Following standard setup procedure in a CMake-compatible IDE should fetch all these dependencies for you, so you shouldn't need to download them separately.)

### Other help

* Cross platform CMake setup created using [Hunter](https://github.com/ruslo/hunter)
* OBJ loader adapted from [opengl-tutorial.org](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/)
