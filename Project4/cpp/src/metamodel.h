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

    void read(const std::string& filename);
    void write() const;
    void save(std::vector<arma::imat>& states,
              std::vector<double>& energies, std::vector<int>& magmoments) const;
    void saveExpectationValues(std::ofstream& file, std::vector<double>&, double T,
                               int numProcessors, int waitNSteps) const;
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    void binaryDump(std::ofstream&, const std::vector<T>&) const;
    template<typename T>
    void binaryDump(std::ofstream&, const std::vector<arma::Mat<T>>&) const;
    // void binaryDump(std::ofstream&, const std::vector<arma::imat>&);
    void setTemperature(double T){temperature = T; beta = 1/T;};

    int          seed        = 1;
    unsigned int N           = 100;          // Lattice size (N x N)
    unsigned int M           = 10000;       // Number of MC Cycles
    double       temperature = 5.0;
    unsigned int n_saves     = 1000;

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

    double Tstart  = 2.0;
    double Tstop   = 3.0;
    double Tnumber = 100;
    double Tstep   = (Tstop - Tstart)/(Tnumber);
};

#endif /* METAMODEL_H */
