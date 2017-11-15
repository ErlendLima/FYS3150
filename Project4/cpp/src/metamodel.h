#ifndef METAMODEL_H
#define METAMODEL_H

#include <iostream>

struct metamodel{
    int seed             = 1233;
    const unsigned int N = 2;          // Lattice size (N x N)
    const unsigned int M = 1000;       // Number of MC Cycles
    double temperature   = 1.0;
    double beta          = 1/temperature;

    unsigned int saveperiod;
    // TODO: INITIAL ORIENTATION SHOULD BE HERE
    std::string basepath   = "../data/";
    std::string energypath = "energies.bin";
    std::string magneticmomentpath = "magneticmoment.bin";
    std::string metadatapath = "metacpp.json";
    std::string evolutionpath = "evolution.bin";
};
void writeMetaData(metamodel&);

#endif /* METAMODEL_H */
