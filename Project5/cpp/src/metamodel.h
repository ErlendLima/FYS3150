#ifndef METAMODEL_H
#define METAMODEL_H

#include <iostream>
#include <armadillo>
#include <string>
#include <fstream>

#define pi 3.1415926535

enum class Method{FORWARD_EULER, BACKWARD_EULER, CRANK_NICOLSON};
class Metamodel
{
public:
    Metamodel(const std::string& basepath,
              const std::string& parameterPath);

    void read(const std::string& filename);
    void write() const;
    void save() const;
    void saveExpectationValues(std::ofstream& file, std::vector<double>&, double T,
                               int numProcessors) const;
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    void binaryDump(std::ofstream&, const std::vector<T>&) const;
    template<typename T>
    void binaryDump(std::ofstream&, const std::vector<arma::Mat<T>>&) const;
    template<typename T>
    void binaryDump(std::ofstream& stream, const arma::Mat<T>&) const;

    // Setters and getters
    double       getDt()     const {return (m_tend-m_tstart)/static_cast<double>(m_tsteps);};
    // The number of x-points are increased by 2 to include the boundary conditions
    double       getDx()     const {return (m_xend-m_xstart)/static_cast<double>(m_xsteps+1);};
    double       getDy()     const {return getDx();};
    double       getXsteps() const {return m_xsteps;};
    double       getTsteps() const {return m_tsteps;};
    double       getAlpha()  const {return getDt()/(getDx()*getDx());};
    unsigned int getDim()    const {return m_dim;};
    Method       getMethod() const {return method;};
    void         setDimension(unsigned int dim);
    void         setBoundaries(double lower, double upper);
    std::tuple<double, double> getBoundaries() const {return std::make_tuple(m_xstart_bound, m_xend_bound);};
    arma::mat&   getU();

    std::function<double(double)> initialCondition;


private:

    unsigned int m_dim   = 1;      // Number of dimensions
    unsigned int m_xsteps; // Number of integration points along the X-axis
    unsigned int m_tsteps; // Number of integration points along the Y-axis

    /* Initial and boundary conditions */
    double m_tstart = 0;
    double m_tend   = 10;

    double m_xstart = 0;
    double m_xend   = 1;

    double m_ystart = 0;
    double m_yend   = 1;

    double m_xstart_bound = 0.0;
    double m_xend_bound = 1.0;

    bool m_hasCreatedU = false;
    arma::mat m_u;

    Method method = Method::FORWARD_EULER;

    std::string  m_basepath     = "../data/";
    std::string  m_metapath     = "meta.json";
    std::string  m_solutionpath = "solution.bin";
    bool         m_parallel     = false;
};

#endif /* METAMODEL_H */
