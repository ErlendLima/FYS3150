#include <iostream>
#include "metamodel.h"
#include "solver.h"

int main(int argc, char const *argv[]) {
    // Read the parameter path from commandline
    // Default to "parameters.json"
    std::string parameterPath = "parameters.json";
    std::string basepath = "../data/";

    if (argc == 3){
        basepath = argv[1];
        parameterPath = argv[2];
    } else if (argc == 2){
        basepath = argv[1];
    }
    else if(argc > 3){
        std::cerr << "Usage: ./solve [basepath] [relative path to parameters]" << std::endl;
        return -1;
    }

    try{
        Metamodel model = Metamodel(basepath, parameterPath);

        Solver solver(model);
        solver.solve();
        std::cout << "Completed" << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

  return 0;
}
