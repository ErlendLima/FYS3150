# Project Four

![Travis CI](https://travis-ci.org/Caronthir/FYS3150.svg?branch=master)
[![Coverage Status](https://coveralls.io/repos/github/Caronthir/FYS3150/badge.svg?branch=master)](https://coveralls.io/github/Caronthir/FYS3150?branch=master)
<a href="https://scan.coverity.com/projects/fys3150">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/14080/badge.svg"/>
</a>

<p align="center">
<b><a href="#overview">Overview</a></b>
|
<b><a href="#analysis-scripts">Analysis Scripts</a></b>
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
![Animation of a 2D Ising model](https://github.com/Caronthir/FYS3150/blob/master/Project4/latex/figures/animation.gif)

## Overview
The aim of [Project 4](https://compphysics.github.io/ComputationalPhysics/doc/Projects/2017/Project4/pdf/Project4.pdf) in FYS3150
is to find numerical solutions to the Ising model in two dimensions.
The reports and associated LaTeX files are available in the `latex` directory.

## Analysis Scripts

There is one python script which analyzes the results from the C++ simulations. Python 3.6 is required.

| Script                  | Function                                                                                                          |
| ------                  | --------                                                                                                          |
| `analyze.py`            | A general script which plots numerous results from the simulations. This includes energy and magnetization as functions of Monte Carlo cycles, expectation values as a function of temperature and a full animation of the spin time development (as seen above).|
## Usage (C++)

Compilation of the C++ program requires C++14, Armadillo version 6.7, CMake version 3.1, MPI and JSONcpp. On Ubuntu these can be installed with
```console
sudo apt-get install cmake liblapack-dev libblas-dev libbost-dev libarmadillo-dev
```

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
The Julia version is not completed, and does not count as a part of this assignment. 
This is left here to stand as a testament to the hubris of students.

The version of Julia required is at least 0.6.0

These can be tested by running
`julia test.jl`.

## Credits
The contributors of this project are Erlend Lima and Frederik Johan Mellbye.

## License
This project is licensed under the terms of the **MIT** license.
You can check out the full license [here](../LICENSE)
