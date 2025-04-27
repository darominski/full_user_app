Basic example of a C++ project depending on a static library for one property. Built with CMake or
as a Vitis project. Tested with Vitis 2022.2

How to use this repository:

I. Clone the repository.

II. CMake build

1. Create the build directory
```
mkdir build
cd build
```

2. Execute CMake:
```
cmake ../ -DTARGET_ARCH=ARM -DCMAKE_PREFIX_PATH=$STATIC_LIB_INSTALL_DIR
cmake --build .
```
For this repository, the STATIC_LIB_INSTALL_DIR could be replaced (or set to): $PWD/../lib/lib.

3. It is possible to run the created `staticTest` application (in QEMU).

The provided static library, compiled based on code from [this repository](https://gitlab.cern.ch/doaromin/basic-static-lib)
using ARM settings. If x86 architecture would be needed for testing, then the static library binary
would need to be updated so that the architecture of the code and the library are consistent.

III. Vitis project build

1. Run the script building the Vitis project:
```
python run_project_build.py -vp $VITIS_PATH -xsa ./inputs/diot_v2.xsa -s ./source -n $application_name
```
where: `$VITIS_PATH` is your path to the Vitis installation, and `$application_name` is your chosen name 
for the application project to be created. 

2. Start Vitis and begin working with the build project
