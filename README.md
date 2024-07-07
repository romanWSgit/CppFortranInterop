# CppFortranInterop

CppFortranInterop is a project focused on interoperability between C++ and Fortran. It provides a C++ interface for eigenvalue decomposition using a Fortran routine from the LAPACK library. The project takes advantage of the Eigen library and is configured to work on an M1 Mac using Accelerate.

## Prerequisites

- A modern C++ compiler supporting the C++17 standard
- A Fortran compiler
- [Eigen library](http://eigen.tuxfamily.org/index.php?title=Main_Page) installed and configured correctly
- LAPACK library, preferably the version included with the [Accelerate framework](https://developer.apple.com/documentation/accelerate) for optimal performance on M1 Macs

## Installation

1. Clone the repository: `git clone https://github.com/yourusername/CppFortranInterop.git`

2. Navigate to the cloned directory: `cd CppFortranInterop`

3. Review the Makefile to ensure Eigen paths and other related variables are correctly set.

4. Build the project using the Makefile: `make`

## Usage

This project is meant as a backend to perform efficient eigen decomposition using Fortran routines. You can include the header files in your source file. There is an example usage in the `main.cpp` file.

## Contributing

Contributions are welcomed. Please open an issue first to discuss the changes you wish to make.

## Author

- Roman Wallner-Silberhubere - Initial work

## License

[LICENCE](LICENCE)