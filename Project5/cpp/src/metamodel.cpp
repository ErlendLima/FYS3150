#include <json/json.h>
#include <json/writer.h>
#include <iostream>
#include <fstream>
#include <type_traits>
#include "math.h"
#include "metamodel.h"


Metamodel::Metamodel(const std::string& basepath,
                     const std::string& parameterPath){
    m_basepath = basepath;
    read(parameterPath);
}
void Metamodel::write() const{
  std::ofstream metafile(m_basepath+m_metapath);
  Json::Value root;
  root["solution"]["dim"]  = Json::arrayValue;
  root["solution"]["type"] = "float64";
  root["solution"]["path"] = "solution.bin";
  root["solution"]["dim"].append(m_tsteps);
  root["solution"]["dim"].append(m_xsteps);
  root["parallel"]      = m_parallel;
  root["t steps"]      = m_tsteps;
  root["x steps"]      = m_xsteps;
  metafile << root << std::endl;
  metafile.close();
}

void Metamodel::read(const std::string& filename) {
    std::ifstream parameters(m_basepath + filename);
    if (!parameters.good())
        throw std::runtime_error("Could not find parameters");

    Json::Value root;
    parameters >> root;
    m_parallel  = root["parallel"].asBool();
    m_xsteps    = root["number of x points"].asInt();
    m_tsteps    = root["number of t points"].asInt();
    setDimension(root["dimensions"].asInt());
    std::cout << "There are " << m_xsteps << " integration points along the x-axis\n"
              << "and " << m_tsteps << " integration points along the t-axis\n";
    setBoundaries(root["lower bound"].asDouble(), root["upper bound"].asDouble());

    // Set the initial condition
    std::string initial = root["initial condition"].asString();
    if (initial == "zero"){
        initialCondition = [](double x){return 0;};
        std::cout << "The initial condition are all 0\n";
    } else if (initial == "sin") {
        initialCondition = [](double x){return sin(pi*x);};
    } else
        throw std::runtime_error("Initial condition not supported.");

    // Use the correct method
    std::string smethod = root["method"].asString();
    if (smethod == "forward"){
        method = Method::FORWARD_EULER;
        std::cout << "Using Forward Euler\n";
    } else if (smethod == "backward"){
        method = Method::BACKWARD_EULER;
        std::cout << "Using Backward Euler\n";
    } else if (smethod == "crank-nicolson"){
        method = Method::CRANK_NICOLSON;
        std::cout << "Using Crank-Nicolson scheme\n";
    } else
        throw std::runtime_error("Method is not supported");
}

void Metamodel::setDimension(unsigned int dim) {
    if(dim == 1 || dim == 2)
        m_dim = dim;
    else
        throw std::runtime_error("Dimension must be either 1 or 2");
}

void Metamodel::setBoundaries(double lower, double upper){
    m_xstart_bound = lower;
    m_xend_bound = upper;
}

arma::mat& Metamodel::getU(){
    if(m_hasCreatedU)
        return m_u;

    m_u = arma::zeros<arma::mat>(m_tsteps, m_xsteps+2);
    m_hasCreatedU = true;

    // Set the initial condition
    arma::vec X = arma::linspace(m_xstart, m_xend, m_xsteps+2);
    for(unsigned int x = 0; x < m_xsteps+2; x++)
        m_u(0, x) = initialCondition(X[x]);

    // Set boundary conditions
    for(unsigned int t = 0; t < m_tsteps; t++){
        m_u(t, 0) = m_xstart_bound;
        m_u(t, m_xsteps+1) = m_xend_bound;
    }
    return m_u;
}

void Metamodel::save() const{
    // Write the metainformation
    write();
    // Write solution u(x, t) to file
    std::ofstream output;
    output.open(m_basepath + m_solutionpath, std::ios::out | std::ios::binary);
    binaryDump(output, m_u);
    output.close();
}

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
void Metamodel::binaryDump(std::ofstream& stream, const std::vector<T>& container) const{
    stream.write((char*)&container[0], container.size()*sizeof(container[0]));
}

template<typename T>
void Metamodel::binaryDump(std::ofstream& stream, const arma::Mat<T>& matrix) const{
    stream.write((char*)&matrix(0,0), m_tsteps*m_xsteps*sizeof(matrix(0,0)));
}

template<typename T>
void Metamodel::binaryDump(std::ofstream& stream, const std::vector<arma::Mat<T>>& container) const{
    for(const auto& matrix: container){
        stream.write((char*)&matrix(0,0), m_tsteps*m_xsteps*sizeof(matrix(0,0)));
    }
}
