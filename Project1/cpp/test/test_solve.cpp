#include <gtest/gtest.h>
#include <armadillo>
#include <memory>
#include "../src/solver.h"

#define TOLERANCE 1e-6
#define LUTOLERANCE 1e-3
// LU has so bad accuracy that it is a joke

using sptr = std::shared_ptr<Solver>;
double constfn(double x)     {return 1.0;}
double constfn_sol(double x) {return -0.5*(x-1)*x;}
double expfn(double x)       {return exp(x);}
double expfn_sol(double x)   {return exp(1)*x-x-exp(x)+1;}

class SolverTest : public ::testing::Test{
protected:
    virtual void SetUp(){
        constSolver = std::make_unique<Solver>(constfn);
        expSolver   = std::make_unique<Solver>(expfn);

        constSolver->setAnalytical(constfn_sol);
        expSolver  ->setAnalytical(expfn_sol);

        constSolver->doSave(false);
        expSolver  ->doSave(false);
    }
    // virtual void TearDown(){}
    sptr constSolver;
    sptr expSolver;

    // Helper function to compare calculated to analytic solution
    bool _approx_equal(arma::vec A, arma::vec B, double tolerance){
        if (arma::numel(A) != arma::numel(B))
            throw std::runtime_error("Input vectors must be of equal size");

        for(unsigned int n = 0; n < arma::numel(A); n++){
            // auto diff =  fabs(A(n)-B(n))/fmax(fabs(A(n)), fabs(B(n)));
            auto diff =  fabs(A(n)-B(n));
            if (diff >= tolerance){
                std::cout << "A,B = " << A(n) << '\t' << B(n) << '\n';
                return false;
            }
        }
        return true;
    }
    bool compareNumericToAnalytic(sptr solver, int steps, double tolerance){
        auto expected = solver->makeDomain(steps);
        expected->transform(solver->getAnalytical());
        auto solution = solver->getSolution();
        return _approx_equal(solution, *expected, tolerance);
    }
};

TEST_F(SolverTest, LU){
    int steps = 1e3;
    constSolver->solve(Method::LU, steps, steps, 10);
    expSolver  ->solve(Method::LU, steps, steps, 10);
    ASSERT_TRUE(compareNumericToAnalytic(constSolver, steps, LUTOLERANCE));
    ASSERT_TRUE(compareNumericToAnalytic(expSolver,   steps, LUTOLERANCE));
}

TEST_F(SolverTest, General){
    int steps = 1e6;
    constSolver->solve(Method::GENERAL, steps, steps, 10);
    expSolver  ->solve(Method::GENERAL, steps, steps, 10);
    ASSERT_TRUE(compareNumericToAnalytic(constSolver, steps, TOLERANCE));
    ASSERT_TRUE(compareNumericToAnalytic(expSolver,   steps, TOLERANCE));
}

TEST_F(SolverTest, Special){
    int steps = 1e6;
    constSolver->solve(Method::SPECIAL, steps, steps, 10);
    expSolver  ->solve(Method::SPECIAL, steps, steps, 10);
    ASSERT_TRUE(compareNumericToAnalytic(constSolver, steps, TOLERANCE));
    ASSERT_TRUE(compareNumericToAnalytic(expSolver,   steps, TOLERANCE));
}
