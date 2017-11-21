#ifndef METAMODEL_H
#define METAMODEL_H

#include <iostream>
#include <armadillo>
#include <string>
#include <fstream>

class Metamodel
{
public:
    Metamodel(){};

    void read(const std::string& filename);
    void write() const;
    void save() const;
    void saveExpectationValues(std::ofstream& file, std::vector<double>&, double T,
                               int numProcessors) const;
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    void binaryDump(std::ofstream&, const std::vector<T>&) const;
    template<typename T>
    void binaryDump(std::ofstream&, const std::vector<arma::Mat<T>>&) const;

    // Setters and getters
    double       getDt() const {return m_dt;};
    double       getDx() const {return m_dx;};
    double       getDy() const {return m_dx;};
    void         setDimension(unsigned int dim);
    unsigned int getDim() const {return m_dim;};

private:

    unsigned int m_dim   = 1;      // Number of dimensions
    double       m_dx    = 0.1;    // Step size for x and y
    double       m_dt    = 0.1;    // Step size for t

    /* Initial and boundary conditions. Unnecessary? */
    double t_start = 0;
    double t_end   = 1;

    double x_start = 0;
    double x_end   = 1;

    double y_start = 0;
    double y_end   = 1;


    std::string  m_basepath   = "../data/";
    std::string  m_solverpath = "data.txt";
    std::string  m_metapath   = "meta.json";
    bool         m_parallel   = false;
};

#endif /* METAMODEL_H */
