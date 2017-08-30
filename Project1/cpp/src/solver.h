#ifndef SOLVER_H
#define SOLVER_H
#include <armadillo>
#include <memory>

enum class Method{GENERAL, SPECIAL, LU};
class Solver
{
public:
    Solver(double (*function)(double));
    virtual ~Solver();
    void setup(unsigned int n);
    void solve(Method, unsigned int low, unsigned int high, unsigned int step);
    void solveGeneral(unsigned int n);
    void solveLU(unsigned int n);
    void save(const std::string& identifier);
    void doSave(bool flag){saveFlag = flag;};
    void setBounds(double lower, double upper){lowerBound = lower; upperBound=upper;};
    void setSavepath(const std::string& path){savepath = path;};
    arma::vec& getSolution(){return solution;};
private:
    arma::vec btilde;
    arma::vec solution;
    double (*fn)(double);
    double lowerBound = 0;
    double upperBound = 0;
    bool saveFlag = true;
    std::string savepath = "";
};


#endif /* SOLVER_H */
