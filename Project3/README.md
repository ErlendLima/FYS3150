# Project Three 

![Travis CI](https://travis-ci.org/Caronthir/FYS3150.svg?branch=master)
[![Coverage Status](https://coveralls.io/repos/github/Caronthir/FYS3150/badge.svg?branch=master)](https://coveralls.io/github/Caronthir/FYS3150?branch=master)
<a href="https://scan.coverity.com/projects/fys3150">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/14080/badge.svg"/>
</a>

<p align="center">
<b><a href="#overview">Overview</a></b>
|
<b><a href="#usage-c">Usage (C++)</a></b>
|
<b><a href="#testing-c">Testing (C++)</a></b>
|
<b><a href="#usage-julia">Usage (Julia)</a></b>
|
<b><a href="#credits">Credits</a></b>
|
<b><a href="#license">License</a></b>
</p>
<br>


<!-- <img height="700px" align="center" src="latex/figures/solarsys.png?raw=true"> -->
![Animation of the solar system](https://github.com/Caronthir/FYS3150/blob/master/Project3/latex/figures/animation.gif)

## Overview
The aim of [Project 3](https://compphysics.github.io/ComputationalPhysics/doc/Projects/2017/Project3/pdf/Project3.pdf) in FYS3150
is to develop code for simulating the solar system using Euler's Forward method and Velocity Verlet.
This is implemented in both C++ and Julia, and further analysis and plotting is done by Python. 
Python is also used for collecting the initial conditions from [NASA](https://ssd.jpl.nasa.gov/?horizons).
The report itself and associated LaTeX files are available in the `latex` directory.


## Usage (C++)

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
Testing is not implemented in the C++ version.
<!-- The framework [Google Test](https://github.com/google/googletest) is used to write unit tests. Google test is -->
<!-- imported as git submodule. To make it work, type -->

<!-- ```console -->
<!-- git submodule init -->
<!-- git submodule update -->
<!-- ``` -->

<!-- Google test is fully integrated into CMake, making it easy to both add and run the tests. In order -->
<!-- to compile the tests, type `cmake -Dtest=ON`. For actually running the tests, run `./runUnitTests` or -->
<!-- `make test`. -->

<!-- Better support for debugging and stricter compiler flags can be turned on using `cmake -DCMAKE_BUILD_TYPE=DEBUG`. -->

## Usage (Julia)
The version of Julia required is at least 0.6.0

These can be tested by running
`julia test.jl`.

## Credits
The contributors of this project are Erlend Lima and Frederik Johan Mellbye.

## License
This project is licensed under the terms of the **MIT** license.
You can check out the full license [here](../LICENSE)
