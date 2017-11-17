#ifndef METAMODEL_H
#define METAMODEL_H

#include <iostream>
#include <armadillo>
#include <string>
#include <fstream>

class Metamodel
{
public:
    Metamodel(){};

    void write() const;
    void save(std::vector<arma::imat>& states,
              std::vector<double>& energies, std::vector<int>& magmoments) const;
    void saveExpectationValues(std::ofstream& file, std::vector<double>&, double T,
                               int numProcessors) const;
    template<typename T>
    void binaryDump(std::ofstream& stream, const std::vector<T>& a) const;
    void binaryDump(std::ofstream& stream, const std::vector<arma::imat>& states);
    void setTemperature(double T){temperature = T; beta = 1/T;};

    int seed             = 1;
    const unsigned int N = 20;          // Lattice size (N x N)
    const unsigned int M = 1000;       // Number of MC Cycles
    double temperature   = 5.0;
    unsigned int n_saves = 1000;

    double       beta               = 1/temperature;
    unsigned int saveperiod         = M/n_saves;
    std::string  initialOrientation = "random"; // Can be random, up, down
    std::string  basepath           = "../data/";
    std::string  energypath         = "energies.bin";
    std::string  magneticmomentpath = "magneticmoment.bin";
    std::string  metadatapath       = "metacpp.json";
    std::string  evolutionpath      = "evolution.bin";
    std::string  solverpath         = "data.txt";
    std::string  metapath           = "meta.json";
    bool         parallel           = false;

    // Settings for parallelized code
    double Tstart = 1.0;
    double Tstop  = 7.0;
    double Tstep  = 0.01;
};

#endif /* METAMODEL_H */
