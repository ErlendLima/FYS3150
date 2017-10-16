#include "parameters.h"
#include "parameter.h"
#include <vector>
#include <iostream>
#include <memory>
#include <map>

using Pair = std::pair<std::string, std::unique_ptr<ParameterInterface>>;

template <typename T>
std::unique_ptr<ParameterInterface> newParam(){
    return std::unique_ptr<ParameterInterface>(new Parameter<T>());
}

template <typename T>
T getParameter();

template <>
int getParameter(){
    return 1;
}

template <>
std::string getParameter(){
    return "wow";
}

int main(int argc, char *argv[])
{
    try{
    Parameters parameters("../../input/parameters.txt");
    std::string name;
    int nx;
    double hz;
    std::cout << parameters.m_doubleparams["hZ"]->get() << std::endl;
    std::cout << parameters.get<double>("hZ") << std::endl;
    std::cout << "cat " << getParameter<int>() << '\t' << getParameter<std::string>() << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
