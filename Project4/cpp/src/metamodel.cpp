#include <json/json.h>
#include <json/writer.h>
#include <iostream>
#include <fstream>
#include "metamodel.h"



void writeMetaData(metamodel& model){
  std::ofstream metafile("../data/meta.json");
  Json::Value root;
  root["evolution"]["dim"]  = Json::arrayValue;
  root["evolution"]["type"] = "int16";
  root["evolution"]["path"] = "evolution.bin";
  root["evolution"]["dim"].append(model.N);
  root["evolution"]["dim"].append(model.N);
  root["evolution"]["dim"].append(model.M);

  root["magnetic moment"]["dim"]  = Json::arrayValue;
  root["magnetic moment"]["type"] = "float64";
  root["magnetic moment"]["path"] = "magneticmoment.bin";
  root["magnetic moment"]["dim"].append(model.M);

  root["energy"]["dim"]  = Json::arrayValue;
  root["energy"]["type"] = "float64";
  root["energy"]["path"] = "energies.bin";
  root["energy"]["dim"].append(model.M);

  root["saveperiod"]    = 1000;
  root["seed"]          = model.seed;
  root["lattice size"]  = model.N;
  root["MC iterations"] = model.M;
  metafile << root << std::endl;
  metafile.close();
  //TODO: ENDRE SAVEPERIOD TIL NUMBER OF SAVES
}
