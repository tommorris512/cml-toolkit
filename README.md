# C Machine Learning Toolkit

A lightweight library of fundamental machine learning models designed for easy integration into other C projects. The toolkit primarily focuses on simplicity, performance and robustness. Check out my [Image Cluster Compressor](https://github.com/tommorris512/image-cluster-compressor) project to see an example of this library in action.

### Models implemented (so far):
- Linear regression
- K-means clustering

## Prerequisites

- Any C compiler, such as `clang` or `gcc`, that supports the C99 standard. (To use compilers other than `clang`, please change the `CC` compiler command variable at the top of the project's Makefile).
- The `make` command to execute the Makefile rules.


## Installation

1. Clone the repository and navigate to the project's root directory:
```bash
git clone https://github.com/tommorris/cml-toolkit.git
cd ./cml-toolkit
```

2. Build the static and shared libraries (`libcml.a`/`libcml.so`) using the included Makefile:
```bash
# Build the shared library file only
make sharedlib

# Build the static library file only
make staticlib

# Build both static and shared library files
make
```
Note that compilation of either static or shared library files will also compile unit test files for each of the models to verify their validity.


## Usage

The generated static or shared library files can be employed alongside the model header files to utilise specific model(s) in a project. The steps are as follows:

1. Include the corresponding header file(s) for the model(s) being used.
2. Link the static or shared library file during the linkage stage of compilation.


Check out my [Image Cluster Compressor](https://github.com/tommorris512/image-cluster-compressor) project to see how this can be achieved.