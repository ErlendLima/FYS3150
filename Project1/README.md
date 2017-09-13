# Project One

<p align="center">
<b><a href="#usage">Usage</a></b>
|
<b><a href="#testing">Testing</a></b>
|
<b><a href="#credits">Credits</a></b>
|
<b><a href="#license">License</a></b>
</p>
<br>

## Usage
The project is divided into source code and the written rapport. The source code
is located across two different directories: analysis and cpp. The 
resulting data produced by the C++ program is read by the analysis code located
in analysis. 

Compilation of the C++ program requires C++14, Armadillo version 6.7 and CMake version 3.1. If the 
requirements are satisfied, the makefile can be created and run with

```console
cmake . && make
```

After initial compilation, typing `make` will make the program. To run the
compiled program, type `./solve`. Optional flags exist, see `./solve -h`.
The resulting data can then be analyzed 
by the python script in the analysis directory using `python analyze.py ../cpp/data`.

The rapport and all supporting material is located in the `latex` directory. 
The document is compatible with PdfLaTeX, LuaTeX and XeLaTeX.

## Testing
The framework [Google Test](https://github.com/google/googletest) is used to write unit tests.
This is fully integrated into CMake, making it easy to both add and run the tests. In order
to compile the tests, type `cmake -Dtest=ON`. For actually running the tests, run `./runUnitTests` or
`make test`.

Better support for debugging and stricter compiler flags can be turned on using `cmake -DCMAKE_BUILD_TYPE=DEBUG`.

## Credits
The contributors of this project are Erlend Lima, Frederik Johan Mellbye and Aram Salihi.

## License
This project is licensed under the terms of the **MIT** license.
You can check out the full license [here](../LICENSE)
