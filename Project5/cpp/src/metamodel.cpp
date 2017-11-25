#include <json/json.h>
#include <json/writer.h>
#include <iostream>
#include <fstream>
#include <cmath>
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
  root["solution"]["type"]   = "float64";
  root["solution"]["format"] = "arma_ascii";
  root["solution"]["path"]   = "solution.bin";
  root["parallel"]           = m_parallel;
  root["t steps"]            = getTsteps();
  root["x steps"]            = getXsteps();
  root["x start"]            = m_xstart;
  root["x end"]              = m_xend;
  root["t start"]            = 0.0;
  root["t end"]              = getTend();
  root["alpha"]              = getAlpha();
  root["dx"]                 = getDx();
  root["dt"]                 = getDt();
  metafile << root << std::endl;
  metafile.close();
}

double Metamodel::getStableDt() const{
    // Calculates the stable step size dt
    // assuming dx and model are specified
    // Since only forward Euler is unstable,
    // the other methods defaults to its dt
    switch (m_method) {
    case Method::CRANK_NICOLSON:
        [[fallthrough]];
    case Method::BACKWARD_EULER:
        [[fallthough]];
    case Method::FORWARD_EULER:
        // Stability condition: Δt/Δx² ≤ 1/2
        return 0.4*getDx()*getDx();
        break;
    }
}

double Metamodel::getDt() const{
    if(m_deducedt)
        return getStableDt();
    else
        return m_dt;
}


double Metamodel::getDx() const{
    return m_dx;
}


unsigned int Metamodel::getXsteps() const{
    // Print warning if the step size does not divide the
    // length of the interval
    double xsteps = (m_xend - m_xstart)/getDx();
    if(xsteps != static_cast<unsigned int>(xsteps)){
        std::cerr << "WARNING: Step size dx does not divide the interval: "
            << xsteps << " != " << static_cast<unsigned int>(xsteps) << std::endl;
    }
    return static_cast<unsigned int>(xsteps);
}


unsigned int Metamodel::getTsteps() const{
    return m_tsteps;
}

double Metamodel::getTend() const {
    return getTsteps()*getDt();
}

void Metamodel::read(const std::string& filename) {
    std::ifstream parameters(m_basepath + filename);
    if (!parameters.good())
        throw std::runtime_error("Could not find parameters");

    Json::Value root;
    parameters >> root;
    m_parallel  = root["parallel"].asBool();
    m_tsteps    = root["number of t points"].asInt();
    m_dt        = root.get("step size dt", -1).asDouble();
    m_dx        = root["step size dx"].asDouble();
    m_xstart    = root["x start"].asDouble();
    m_xend      = root["x end"].asDouble();
    setDimension(root["dimensions"].asInt());
    setBoundaries(root["lower bound"].asDouble(), root["upper bound"].asDouble());

    // If only dx is set, deduce dt
    if(m_dt < 0){
        std::cout << "Deducing dt from stability condition" << std::endl;
        m_deducedt = true;
    }
    std::cout << "There are " << getXsteps() << " integration points along the x-axis on the interval ["
              << m_xstart << ", " << m_xend << "] with dx = " << getDx()
              << "\nand " << m_tsteps << " integration points along the t-axis on the interval ["
              << 0.0 << ", " << getTend() << "] with dt = " << getDt() << std::endl;

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
    std::string method = root["method"].asString();
    if (method == "forward"){
        m_method = Method::FORWARD_EULER;
        std::cout << "Using Forward Euler\n";
    } else if (method == "backward"){
        m_method = Method::BACKWARD_EULER;
        std::cout << "Using Backward Euler\n";
    } else if (method == "crank-nicolson"){
        m_method = Method::CRANK_NICOLSON;
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

    m_u = arma::zeros<arma::mat>(getTsteps(), getXsteps()+2);
    m_hasCreatedU = true;

    // Set the initial condition
    arma::vec X = arma::linspace(m_xstart, m_xend, getTsteps()+2);
    for(unsigned int x = 0; x < getXsteps()+2; x++)
        m_u(0, x) = initialCondition(X[x]);

    // Set boundary conditions
    for(unsigned int t = 0; t < m_tsteps; t++){
        m_u(t, 0) = m_xstart_bound;
        m_u(t, getXsteps()+1) = m_xend_bound;
    }
    return m_u;
}

void Metamodel::save() const{
    // Write the metainformation
    write();
    // Write solution u(x, t) to file
    // std::ofstream output;
    m_u.save(m_basepath+m_solutionpath, arma::raw_ascii);
    // output.open(m_basepath + m_solutionpath, std::ios::out | std::ios::binary);
    // binaryDump(output, m_u);
    // output.close();
}

template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
void Metamodel::binaryDump(std::ofstream& stream, const std::vector<T>& container) const{
    stream.write((char*)&container[0], container.size()*sizeof(container[0]));
}

template<typename T>
void Metamodel::binaryDump(std::ofstream& stream, const arma::Mat<T>& matrix) const{
    stream.write((char*)&matrix(0,0), getTsteps()*getXsteps()*sizeof(matrix(0,0)));
}

template<typename T>
void Metamodel::binaryDump(std::ofstream& stream, const std::vector<arma::Mat<T>>& container) const{
    for(const auto& matrix: container){
        stream.write((char*)&matrix(0,0), getTsteps()*getXsteps()*sizeof(matrix(0,0)));
    }
}
