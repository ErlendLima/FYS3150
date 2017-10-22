# Project Three 

![Travis CI](https://travis-ci.org/Caronthir/FYS3150.svg?branch=master)

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


<!-- <img height="700px" align="center" src="latex/figures/solarsys.png?raw=true"> -->
[![Animation of the solar system](https://github.com/Caronthir/FYS3150/blob/master/Project3/latex/figures/animation.gif)]

## Usage (C++)
The project is divided into source code and the written rapport. The source code
is located across three different directories: analysis, cpp and julia. The
resulting data produced by the C++ program is read by the analysis code located
in analysis, while julia does both its own computations and its own plotting.

Compilation of the C++ program requires C++14, Armadillo version 6.7, CMake version 3.1 and JSONcpp. On Ubuntu these can be installed with
```console
sudo apt-get install cmake liblapack-dev libblas-dev libbost-dev libarmadillo-dev libjsoncpp-1 libjsoncpp-dev
```
Note that the path to jsoncpp is not standardized. This program assumes it lies in `<include path>/jsoncpp/json/json.h`. 
If you get a compilation error, create a symlink from the path to your `json.h` to `jsoncpp/json/json.h`.

If the
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

## Testing (C++)
The framework [Google Test](https://github.com/google/googletest) is used to write unit tests. Google test is
imported as git submodule. To make it work, type

```console
git submodule init
git submodule update
```

Google test is fully integrated into CMake, making it easy to both add and run the tests. In order
to compile the tests, type `cmake -Dtest=ON`. For actually running the tests, run `./runUnitTests` or
`make test`.

Better support for debugging and stricter compiler flags can be turned on using `cmake -DCMAKE_BUILD_TYPE=DEBUG`.

## Usage (Julia)
The version of Julia required is at least 0.6.0

These can be tested by running
`julia test.jl`.

## Credits
The contributors of this project are Erlend Lima and Frederik Johan Mellbye.

## License
This project is licensed under the terms of the **MIT** license.
You can check out the full license [here](../LICENSE)
