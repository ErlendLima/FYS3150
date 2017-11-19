#include <iostream>
#include "metamodel.h"
#include "solver.h"

int main(int argc, char const *argv[]) {
    // Read the parameter path from commandline
    // Default to "parameters.json"
    std::string parameterPath = "parameters.json";
    if (argc == 2)
        parameterPath = argv[1];
    parameterPath = "../data/" + parameterPath;

    try{
        Metamodel model = Metamodel();
        model.read(parameterPath);
        solveSystem(model);
    } catch (std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
