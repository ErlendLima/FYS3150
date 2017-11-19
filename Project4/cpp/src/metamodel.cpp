#include <json/json.h>
#include <json/writer.h>
#include <iostream>
#include <fstream>
#include <type_traits>
#include "metamodel.h"

void Metamodel::write() const{
  std::ofstream metafile(basepath+metapath);
  Json::Value root;
  root["evolution"]["dim"]  = Json::arrayValue;
  root["evolution"]["type"] = "int64";
  root["evolution"]["path"] = "evolution.bin";
  root["evolution"]["dim"].append(M);
  root["evolution"]["dim"].append(N);
  root["evolution"]["dim"].append(N);

  root["magnetic moment"]["dim"]  = Json::arrayValue;
  root["magnetic moment"]["type"] = "int32";
  root["magnetic moment"]["path"] = "magneticmoment.bin";
  root["magnetic moment"]["dim"].append(M);

  root["flips"]["dim"]  = Json::arrayValue;
  root["flips"]["type"] = "int32";
  root["flips"]["path"] = flippath;
  root["flips"]["dim"].append(M);

  root["energy"]["dim"]  = Json::arrayValue;
  root["energy"]["type"] = "float64";
  root["energy"]["path"] = "energies.bin";
  root["energy"]["dim"].append(M);

  root["saveperiod"]    = 1000;
  root["seed"]          = seed;
  root["lattice size"]  = N;
  root["MC iterations"] = M;
  root["parallel"]      = parallel;
  metafile << root << std::endl;
  metafile.close();
  //TODO: ENDRE SAVEPERIOD TIL NUMBER OF SAVES
}

void Metamodel::read(const std::string& filename) {
    std::ifstream parameters(filename);
    if (!parameters.good())
        throw std::runtime_error("Could not find parameters");
    Json::Value root;
    parameters >> root;
    Tstart             = root["Tstart"].asDouble();
    Tstop              = root["Tstop"].asDouble();
    Tnumber            = root["Tnumber"].asDouble();
    Tstep              = (Tstop - Tstart)/Tnumber;
    parallel           = root["parallel"].asBool();
    seed               = root["seed"].asInt();
    N                  = root["lattice size"].asInt();
    M                  = root["monte carlo cycles"].asInt();
    initialOrientation = root["initial orientation"].asString();
}

void Metamodel::save(std::vector<arma::imat>& states, std::vector<double>& energies,
                     std::vector<int>& magmoments, std::vector<int>& nFlips) const{
  // Write energy to file
  std::ofstream energyStream;
  energyStream.open(basepath + energypath, std::ios::out | std::ios::binary);
  binaryDump(energyStream, energies);
  energyStream.close();

  // Write magnetic moment to file
  std::ofstream magmom;
  magmom.open(basepath + magneticmomentpath, std::ios::out | std::ios::binary);
  binaryDump(magmom, magmoments);
  magmom.close();

  // Write system states to file
  std::ofstream evoStream;
  evoStream.open(basepath + evolutionpath, std::ios::out | std::ios::binary);
  binaryDump(evoStream, states);
  evoStream.close();

  // Write number of accepted flips each MC cycle to file
  std::ofstream flips;
  flips.open(basepath + flippath, std::ios::out | std::ios::binary);
  binaryDump(flips, nFlips);
  flips.close();
}

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
void Metamodel::binaryDump(std::ofstream& stream, const std::vector<T>& container) const{
    stream.write((char*)&container[0], M*sizeof(container[0]));
}

template<typename T>
void Metamodel::binaryDump(std::ofstream& stream, const std::vector<arma::Mat<T>>& container) const{
    for(const auto& matrix: container){
        stream.write((char*)&matrix(0,0), N*N*sizeof(matrix(0,0)));
    }
}

void Metamodel::saveExpectationValues(std::ofstream& stream, std::vector<double>& expVals,
                                      double T, int numProcessors) const{
  // Dumps values for a given temperature to stream.
  unsigned int Mprime   = M*numProcessors;
  double       factor   = 1.0/static_cast<double>(Mprime);
  double       spinNorm = 1.0/static_cast<double>(N*N);

  double expectE        = expVals[0]*factor;
  double expectESquared = expVals[1]*factor;
  double expectM        = expVals[2]*factor;
  double expectMSquared = expVals[3]*factor;
  double expectMFabs    = expVals[4]*factor;
  double n_flips        = expVals[5]*factor;

  double varE = (expectESquared - expectE*expectE)*spinNorm;
  double varM = (expectMSquared - expectMFabs*expectMFabs)*spinNorm;

  double Cv  = varE/T/T;
  double sus = varM/T;

  stream << T << " "
         << expectE*spinNorm << " "
         << expectESquared*spinNorm << " "
         << varE << " "
         << Cv << " "
         << expectM*spinNorm <<  " "
         << expectMSquared*spinNorm << " "
         << expectMFabs*spinNorm << " "
         << varM << " "
         << sus << " "
         << n_flips << "\n";
}
