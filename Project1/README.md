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
is located across four different directories: analysis, C++, Fortran and Julia. The 
resulting data produced by the C++ and Fortran programs are read by the analysis code located
in analysis. Since Julia is a high level language, both data generation and visualization
is done with Julia. 

The reason for three languages is mainly to get used to the different languages, and
as an insurance for writing correct code as all three implementations should agree.

Compilation of the C++ program requires the Armadillo library and CMake. If the 
requirements are satisfied, the makefile can be created and run with

```console
cmake . && make
```

After initial compilation, typing `make` will make the program. To run the
compiled program, type `./solve`. The resulting data can then be analyzed 
by the python script in the analysis directory using `python analyze.py ../cpp`.

For running the Julia script, type 

```console
julia solve.jl
```

The rapport and all supporting material is located in the `latex` directory. As 
science is based on reproducibility, the latex document is written in PythonTeX, 
allowing arbitrary Python code to be run from the same document. This allows
all of the simulation data, tables, figures, etc to be re-generated on the fly. One
downside of this is that it makes compilation of the latex file more involved. 

The document is compatible with pdflatex, luatex and xelatex. Pdflatex is used for demonstration
purposes:

```console
pdflatex rapport.tex
pythontex rapport.tex
pdflatex rapport.tex
```

## Testing
The framework [Google Test](https://github.com/google/googletest) is used to write unit tests.
This is fully integrated into CMake, making it easy to both add and run the tests. In order
to compile the tests, type `cmake -Dtest=ON`. For actually running the tests, run `./runUnitTests` or
`make tests`.

## Credits
The contributors of this project are Erlend Lima, Frederik Johan Mellbye and Aram Salihi.

## License
This project is licensed under the terms of the **MIT** license.
You can check out the full license [here](../LICENSE)
