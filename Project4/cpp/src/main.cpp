#include <iostream>
#include "metamodel.h"
#include "solver.h"

int main(int argc, char const *argv[]) {
    try{
    Metamodel model = Metamodel();
    model.read("../data/parameters.json");
    solveSystem(model);
    } catch (std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
