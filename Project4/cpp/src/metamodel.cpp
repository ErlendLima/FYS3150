#include <json/json.h>
#include<json/writer.h>
#include "ising.h"
#include "metamodel.h"

void writeMetaData(parameters& params){
  std::ofstream metafile("../data/meta.json");
  Json::Value root;
  root["evolution"]["dim"]  = Json::arrayValue;
  root["evolution"]["type"] = "int16";
  root["evolution"]["path"] = "evolution.bin";
  root["evolution"]["dim"].append(params.N);
  root["evolution"]["dim"].append(params.N);
  root["evolution"]["dim"].append(params.M);

  root["magnetic moment"]["dim"]  = Json::arrayValue;
  root["magnetic moment"]["type"] = "float64";
  root["magnetic moment"]["path"] = "magneticmoment.bin";
  root["magnetic moment"]["dim"].append(params.M);

  root["energy"]["dim"]  = Json::arrayValue;
  root["energy"]["type"] = "float64";
  root["energy"]["path"] = "energies.bin";
  root["energy"]["dim"].append(params.M);

  root["saveperiod"]    = 1000;
  root["seed"]          = params.seed;
  root["lattice size"]  = params.N;
  root["MC iterations"] = params.M;
  metafile << root << std::endl;
  metafile.close();
  //TODO: ENDRE SAVEPERIOD TIL NUMBER OF SAVES
}
