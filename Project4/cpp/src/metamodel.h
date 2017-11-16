#ifndef METAMODEL_H
#define METAMODEL_H

#include <iostream>
#include <armadillo>
#include <string>

struct metamodel{
    int seed             = 1;
    const unsigned int N = 20;          // Lattice size (N x N)
    const unsigned int M = 10000;       // Number of MC Cycles
    double temperature   = 5.0;
    unsigned int n_saves = 1000;

    double beta                    = 1/temperature;
    unsigned int saveperiod        = M/n_saves;
    std::string initialOrientation = "random"; // Can be random, up, down
    std::string basepath           = "../data/";
    std::string energypath         = "energies.bin";
    std::string magneticmomentpath = "magneticmoment.bin";
    std::string metadatapath       = "metacpp.json";
    std::string evolutionpath      = "evolution.bin";

    double Tstart = 1.0;
    double Tstop  = 7.0;
    double Tstep  = 0.2;
};
void writeMetaData(metamodel&);
void save(metamodel&, std::vector<arma::imat>& states,
          std::vector<double>& energies, std::vector<int>& magmoments);

#endif /* METAMODEL_H */
