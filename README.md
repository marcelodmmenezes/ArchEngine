# TCC

## Developing a minimalist game engine for learning purposes

Tested compilation:
* MSVC 14.13.26128 on Windows 10
* GCC 5.4.0 on Ubuntu 16.04 LTS

This project uses the [Bullet Physics library](http://bulletphysics.org/wordpress/).
Download and build the library at the desired folder.
* On Visual Studio add the library src and lib folders in the project configurations:
    * C/C++ -> General -> Additional Include Directories
    * Linker -> General -> Additional Library Directories
* On Ubuntu just change the provided Makefile's CPLUS_INCLUDE_PATH and LIBRARY_PATH variables.