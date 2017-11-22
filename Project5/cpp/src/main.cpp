#include <iostream>
#include "metamodel.h"
#include "solver.h"

int main(int argc, char const *argv[]) {
    // Read the parameter path from commandline
    // Default to "parameters.json"
    std::string parameterPath = "parameters.json";
    if (argc == 2)
        parameterPath = argv[1];
    else if(argc > 2){
        std::cerr << "Usage: ./solve [path to parameters]" << std::endl;
        return -1;
    }
    parameterPath = "../data/" + parameterPath;

    try{
        Metamodel model = Metamodel();
        model.read(parameterPath);

        Solver solver(model);
        solver.solve();
        std::cout << "Completed" << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

  return 0;
}
