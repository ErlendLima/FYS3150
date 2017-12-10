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
    double       getDt()     const;
    // The number of x-points are increased by 2 to include the boundary conditions
    double       getDx()     const;
    double       getDy()     const {return getDx();};
    unsigned int getXsteps() const;
    unsigned int getTsteps() const;
    double       getAlpha()  const {return getDt()/(getDx()*getDx());};
    unsigned int getDim()    const {return m_dim;};
    Method       getMethod() const {return m_method;};
    void         setDimension(unsigned int dim);
    void         setBoundaries(double lower, double upper);
    double       getStableDt() const;
    double       getTend()   const;
    std::tuple<double, double> getBoundaries() const {return std::make_tuple(m_xstart_bound, m_xend_bound);};
    arma::mat&   getU();

    std::function<double(double)> initialCondition;


private:

    unsigned int m_dim   = 1;      // Number of dimensions

    /* Step sizes. Dt is deduced from stability condition if not specified */
    double       m_dt = -1;     // The step size in time
    double       m_dx = -1;     // The step size in the spacial dimension

    /* Number of integration points in the t direction */
    unsigned int m_tsteps;

    /* Initial and boundary conditions */
    double m_xstart = 0;
    double m_xend   = 1;

    double m_ystart = 0;
    double m_yend   = 1;

    double m_xstart_bound = 0.0;
    double m_xend_bound = 1.0;

    bool m_hasCreatedU = false;
    arma::mat m_u;

    Method m_method = Method::FORWARD_EULER;
    bool   m_deducedt = false;

    std::string  m_basepath     = "../data/";
    std::string  m_metapath     = "meta.json";
    std::string  m_solutionpath = "solution.bin";
    std::string  m_initialCondition;
    bool         m_parallel     = false;
};

#endif /* METAMODEL_H */
