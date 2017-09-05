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
    std::unique_ptr<arma::vec> makeBtilde(unsigned int n);
    std::unique_ptr<arma::vec> makeDomain(unsigned int n);
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
    void setAnalytical(double (*function)(double)){fnAnalytical = function;};
    std::function<double(double)> getAnalytical(){return fnAnalytical;};
    arma::vec& getSolution(){return solution;};
private:
    arma::vec solution;
    double (*fn)(double);
    double (*fnAnalytical)(double);
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
