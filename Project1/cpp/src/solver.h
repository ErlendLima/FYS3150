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
    int solve(Method, unsigned int low, unsigned int high, unsigned int step);
    void solveGeneral(unsigned int n);
    void solveSpecial(unsigned int n);
    void solveLU(unsigned int n);
    void calculateError(unsigned int n_start, unsigned int n_stop, unsigned int n_step);
    void save(const std::string& identifier);
    void doSave(bool flag){saveFlag = flag;};
    void setBounds(double lower, double upper){lowerBound = lower; upperBound=upper;};
    void setSavepath(const std::string& path){savepath = path;};
    void setRepetitions(unsigned int r){repetitions = r;};
    arma::vec& getSolution(){return solution;};
    arma::vec& getDomain(){return domain;};
private:
    arma::vec btilde;
    arma::vec solution;
    arma::vec domain;
    double (*fn)(double);
    double lowerBound = 0;
    double upperBound = 0;
    bool saveFlag = true;
    std::string savepath = "";
    unsigned int repetitions = 1;
    std::chrono::high_resolution_clock::time_point startWallTime;
    double startCPUTime;

    void startTiming();
    void endTiming();
};


#endif /* SOLVER_H */
