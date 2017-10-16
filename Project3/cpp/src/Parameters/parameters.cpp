#ifndef PARAMETERS_CPP
#define PARAMETERS_CPP

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <map>
#include <memory>
#include "parameters.h"
#include "parameter.h"

template <typename T>
std::unique_ptr<Parameter<T>> newParam(const std::string &name){
    return std::unique_ptr<Parameter<T>>(new Parameter<T>(name));
}

Parameters::Parameters(const std::string &filenameConfig){
    constructMap();
    readParameters(filenameConfig);
    checkVersion();
    checkThatAllParametersAreSet();
    m_infileParameters.close();
}

Parameters::~Parameters(){

}

template <>
void Parameters::addParameter<double>(std::string name){
    m_doubleparams[name] = newParam<double>(name);
}

template <>
void Parameters::addParameter<int>(std::string name){
    m_intparams[name] = newParam<int>(name);
}

template <>
void Parameters::addParameter<std::string>(std::string name){
    m_stringparams[name] = newParam<std::string>(name);
}

template <>
void Parameters::addParameter<bool>(std::string name){
    m_boolparams[name] = newParam<bool>(name);
}

template <>
int Parameters::get(std::string name){
    if (m_intparams.find(name) == m_intparams.end()){
        std::cerr << "Tried to get a non-existent int paramter: " << name << std::endl;
        throw std::exception();
    }
    return m_intparams[name]->get();
}

template <>
double Parameters::get(std::string name){
    if (m_doubleparams.find(name) == m_doubleparams.end()){
        std::cerr << "Tried to get a non-existent double paramter: " << name << std::endl;
        throw std::exception();
    }
    return m_doubleparams[name]->get();
}

template <>
std::string Parameters::get(std::string name){
    if (m_stringparams.find(name) == m_stringparams.end()){
        std::cerr << "Tried to get a non-existent string paramter: " << name << std::endl;
        throw std::exception();
    }
    return m_stringparams[name]->get();
}

template <>
bool Parameters::get(std::string name){
    if (m_boolparams.find(name) == m_boolparams.end()){
        std::cerr << "Tried to get a non-existent bool paramter: " << name << std::endl;
        throw std::exception();
    }
    return m_boolparams[name]->get();
}

void Parameters::constructMap(){
    addParameter<double>("version");
    addParameter<std::string>("frictionsystem");
    addParameter<int>("nx");
    addParameter<int>("ny");
    addParameter<int>("nt");
    addParameter<int>("releaseTime");
    addParameter<int>("drivingTime");
    addParameter<double>("fn");
    addParameter<int>("ns");
    addParameter<double>("tRmean");
    addParameter<double>("tRstd");
    addParameter<double>("d");
    addParameter<double>("E");
    addParameter<double>("k");
    addParameter<double>("nu");
    addParameter<double>("hZ");
    addParameter<double>("density");
    addParameter<double>("step");
    addParameter<double>("mud");
    addParameter<double>("mus");
    addParameter<double>("absDampCoeff");
    addParameter<double>("relVelDampCoeff");
    addParameter<double>("alpha");
    addParameter<int>("snapshotstart");
    addParameter<int>("snapshotbuftime");
    addParameter<std::string>("outputpath");
    addParameter<std::string>("latticefilename");
    addParameter<std::string>("dumpfilename");
    addParameter<int>("grooveSize");
    addParameter<int>("grooveHeight");
    addParameter<double>("vD");
    addParameter<double>("pK");
    addParameter<int>("pusherStartHeight");
    addParameter<int>("pusherEndHeight");
    addParameter<double>("beamMass");
    addParameter<double>("beamAngle");
    addParameter<int>("beamRotTime");
    addParameter<bool>("writeInterfacePosition");
    addParameter<bool>("writeInterfaceVelocity");
    addParameter<bool>("writeInterfaceAttachedSprings");
    addParameter<bool>("writeInterfaceNormalForce");
    addParameter<bool>("writeInterfaceShearForce");
    addParameter<bool>("writeAllPosition");
    addParameter<bool>("writeAllVelocity");
    addParameter<bool>("writeAllEnergy");
    addParameter<bool>("writeAllForce");
    addParameter<bool>("writePusherForce");
    addParameter<bool>("writeXYZ");
    addParameter<bool>("writeBeamTorque");
    addParameter<bool>("writeBeamShearForce");
    addParameter<int>("freqInterfacePosition");
    addParameter<int>("freqInterfaceVelocity");
    addParameter<int>("freqInterfaceAttachedSprings");
    addParameter<int>("freqInterfaceNormalForce");
    addParameter<int>("freqInterfaceShearForce");
    addParameter<int>("freqAllPosition");
    addParameter<int>("freqAllVelocity");
    addParameter<int>("freqAllEnergy");
    addParameter<int>("freqAllForce");
    addParameter<int>("freqPusherForce");
    addParameter<int>("freqXYZ");
    addParameter<int>("freqBeamTorque");
    addParameter<int>("freqBeamShearForce");
}


void Parameters::readParameters(std::string filenameConfig){
    m_infileParameters.open(filenameConfig);

    if (!m_infileParameters) {
        std::cerr << "The config file " << filenameConfig << " could not be opened" << std::endl;
        throw std::runtime_error("The config could not be read");
    }

    std::string line;
    while (getline(m_infileParameters, line)) {
        // Cut the string into substrings
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             back_inserter(tokens));

        // Parse the substrings
        if (tokens.size() == 0 || tokens[0][0] == '\n' || tokens[0][0] == '#')
          continue;
        if (m_intparams.find(tokens[0]) != m_intparams.end()) {
            m_intparams[tokens[0]]->read(tokens[1]);
        }
        else if (m_doubleparams.find(tokens[0]) != m_doubleparams.end()){
            m_doubleparams[tokens[0]]->read(tokens[1]);
        }
        else if (m_stringparams.find(tokens[0]) != m_stringparams.end()){
            m_stringparams[tokens[0]]->read(tokens[1]);
        }
        else if (m_boolparams.find(tokens[0]) != m_boolparams.end()){
            m_boolparams[tokens[0]]->read(tokens[1]);
        }
        else {
            std::cerr << "Unknown token in parameters: " << tokens[0] << std::endl;
            continue;
        }
    }
}

void Parameters::checkThatAllParametersAreSet(){
    for (auto& param: m_intparams){
        if (!param.second->isSet()){
            std::cerr << param.first << " is not set.";
            throw std::exception();
        }
    }
    for (auto& param: m_doubleparams){
        if (!param.second->isSet()){
            std::cerr << param.first << " is not set.";
            throw std::exception();
        }
    }
    for (auto& param: m_stringparams){
        if (!param.second->isSet()){
            std::cerr << param.first << " is not set.";
            throw std::exception();
        }
    }
    for (auto& param: m_boolparams){
        if (!param.second->isSet()){
            std::cerr << param.first << " is not set.";
            throw std::exception();
        }
    }
}

void Parameters::checkVersion() {
    double version;
    try {
        version = get<double>("version");
    } catch (std::exception &ex) {
        std::stringstream msg;
        msg << "Version incompatible with parameters. Expected version " << VERSION << ", got none." << std::endl;
        throw std::runtime_error(msg.str());
    }
    if (version != VERSION){
        std::stringstream msg;
        msg << "Version incompatible with parameters. Expected version " << VERSION << ", got " << version << std::endl;
        throw std::runtime_error(msg.str());
    }
}

std::ostream & operator<<(std::ostream &os, const Parameters &self){
    for(auto& param: self.m_intparams)
        os << *param.second << '\n';
    for(auto& param: self.m_doubleparams)
        os << *param.second << '\n';
    for(auto& param: self.m_stringparams)
        os << *param.second << '\n';
    for(auto& param: self.m_boolparams)
        os << *param.second << '\n';
    return os;
}
#endif /* PARAMETERS_CPP */
