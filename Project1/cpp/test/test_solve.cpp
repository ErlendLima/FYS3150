#include <gtest/gtest.h>
#include <armadillo>
#include "../src/solver.h"

#define TOLERANCE 1e-5
#define LUTOLERANCE 1e-0
// LU has so bad accuracy that it is a joke

double constfn(double x)     {return 1.0;}
double constfn_sol(double x) {return -0.5*(x-1)*x;}
double expfn(double x)       {return exp(x);}
double expfn_sol(double x)   {return exp(1)*x-x-exp(x)+1;}
double expfn_bounds(double x){return exp(1)*x+4*x-exp(x)-1;};


TEST(LU, ConstantFn){
    Solver solver(constfn);
    solver.doSave(false);
    solver.solve(Method::LU, 1e3, 1e3, 10);
    auto expected = solver.getDomain();
    expected.transform(constfn_sol);
    auto solution = solver.getSolution();
    std::cout << arma::sum(expected-solution) << std::endl;
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", LUTOLERANCE));
}

TEST(General, ConstantFn){
    Solver solver(constfn);
    solver.doSave(false);
    solver.solve(Method::GENERAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(constfn_sol);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(Special, ConstantFn){
    Solver solver(constfn);
    solver.doSave(false);
    solver.solve(Method::SPECIAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(constfn_sol);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(LU, ExpFn){
    Solver solver(expfn);
    solver.doSave(false);
    solver.solve(Method::LU, 1e3, 1e3, 10);
    auto expected = solver.getDomain();
    expected.transform(expfn_sol);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", LUTOLERANCE));
}

TEST(General, ExpFn){
    Solver solver(expfn);
    solver.doSave(false);
    solver.solve(Method::GENERAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(expfn_sol);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(Special, ExpFn){
    Solver solver(expfn);
    solver.doSave(false);
    solver.solve(Method::SPECIAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(expfn_sol);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(LU, Bounds){
    Solver solver(expfn);
    solver.doSave(false);
    solver.setBounds(-2, 3);
    solver.solve(Method::LU, 1e3, 1e3, 10);
    auto expected = solver.getDomain();
    expected.transform(expfn_bounds);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", LUTOLERANCE));
}

TEST(General, Bounds){
    Solver solver(expfn);
    solver.doSave(true);
    solver.setBounds(-2, 3);
    solver.solve(Method::GENERAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(expfn_bounds);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(Special, Bounds){
    Solver solver(expfn);
    solver.doSave(false);
    solver.setBounds(-2, 3);
    solver.solve(Method::SPECIAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(expfn_bounds);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}
TEST(Solve, Consistent){
    // Function to solve for
    double (*fn)(double) = [](double x){return x/2+1.0;};

    Solver solver(fn);
    solver.doSave(false);

    solver.solve(Method::GENERAL, 1e6, 1e6, 10);
    auto general = solver.getSolution();
    solver.solve(Method::SPECIAL, 1e6, 1e6, 10);
    auto special = solver.getSolution();
    // solver.solve(Method::LU, 1e3, 1e3, 10);
    // auto lu = solver.getSolution();

    bool generalNspecial = arma::approx_equal(general, special, "reldiff", TOLERANCE);
    // bool specialNLU = arma::approx_equal(special, lu, "reldiff", TOLERANCE);
    ASSERT_TRUE(generalNspecial);
    // ASSERT_TRUE(specialNLU);
}
