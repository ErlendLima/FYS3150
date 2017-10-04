# Project Two
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

## Usage (C++)
The project is divided into source code and the written rapport. The source code
is located across three different directories: analysis, cpp and julia. The 
resulting data produced by the C++ program is read by the analysis code located
in analysis, while julia does both its own computations and its own plotting.

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

## Testing (C++)
The framework [Google Test](https://github.com/google/googletest) is used to write unit tests.
This is fully integrated into CMake, making it easy to both add and run the tests. In order
to compile the tests, type `cmake -Dtest=ON`. For actually running the tests, run `./runUnitTests` or
`make test`.

Better support for debugging and stricter compiler flags can be turned on using `cmake -DCMAKE_BUILD_TYPE=DEBUG`.

## Usage (Julia)
The version of Julia required is 0.6.0, along with the packages PyPlot, LaTeXStrings and PyCall. If 
these are not available on your system, install them by running for example `Pkg.add("PyPlot")` in
Julia's REPL. 

The most important files are `schroedinger.jl` and `jacobi.jl`. These are modules which implement Jacobi's 
algorithm and methods from solving the Hamiltonian with several potentials. These can be tested by running 
`julia test.jl`. The other scripts are only for
plotting the results. `interactions.jl` plots two plots comparing the ground state and first excited state of
one/two electrons which do and do not interact through the Coulomb force, `timejacobi.jl` runs
the Jacobi algorithm several times, recording the time and memory taken and plots the result together
with equivalent measurements from the C++ program, `wave.jl` creates a rather elegant visualization
of the potentials and their solutions, and `omegas.jl` compares the eigenvalues for different 
oscillator frequencies with those given in the article of M. Taut. 

## Credits
The contributors of this project are Erlend Lima and Frederik Johan Mellbye.

## License
This project is licensed under the terms of the **MIT** license.
You can check out the full license [here](../LICENSE)
