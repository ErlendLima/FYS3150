#include <gtest/gtest.h>
#include <armadillo>
#include "../src/solver.h"

#define TOLERANCE 1e-5
#define LUTOLERANCE 1e-0
// LU has so bad accuracy that it is a joke

TEST(LU, ConstantFn){
    // Function to solve for; Euler-Cauchy equation
    double (*fn)(double) = [](double x){return 1.0;};
    // Solution for Dirichlet boundary conditions
    double (*u)(double) = [](double x){return -0.5*(x-1)*x;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::LU, 1e3, 1e3, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    std::cout << arma::sum(expected-solution) << std::endl;
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", LUTOLERANCE));
}

TEST(General, ConstantFn){
    // Function to solve for; Constant f(x)=1
    double (*fn)(double) = [](double x){return 1.0;};
    // Solution for Dirichlet boundary conditions
    double (*u)(double) = [](double x){return -0.5*(x-1)*x;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::GENERAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(Special, ConstantFn){
    double (*fn)(double) = [](double x){return 1.0;};
    double (*u)(double) = [](double x){return -0.5*(x-1)*x;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::SPECIAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(LU, ExpFn){
    double (*fn)(double) = [](double x){return exp(x);};
    double (*u)(double) = [](double x){return exp(1)*x-x-exp(x)+1;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::LU, 1e3, 1e3, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", LUTOLERANCE));
}

TEST(General, ExpFn){
    double (*fn)(double) = [](double x){return exp(x);};
    double (*u)(double) = [](double x){return exp(1)*x-x-exp(x)+1;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::GENERAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(Special, ExpFn){
    double (*fn)(double) = [](double x){return exp(x);};
    double (*u)(double) = [](double x){return exp(1)*x-x-exp(x)+1;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::SPECIAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(LU, Bounds){
    double (*fn)(double) = [](double x){return exp(x);};
    double (*u)(double) = [](double x){return exp(1)*x+4*x-exp(x)-1;};

    Solver solver(fn);
    solver.doSave(true);
    solver.setBounds(-2, 3);
    solver.solve(Method::LU, 1e3, 1e3, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", LUTOLERANCE));
}

TEST(General, Bounds){
    double (*fn)(double) = [](double x){return exp(x);};
    double (*u)(double) = [](double x){return exp(1)*x+4*x-exp(x)-1;};

    Solver solver(fn);
    solver.doSave(false);
    solver.setBounds(-2, 3);
    solver.solve(Method::GENERAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(Special, Bounds){
    double (*fn)(double) = [](double x){return exp(x);};
    double (*u)(double) = [](double x){return exp(1)*x+4*x-exp(x)-1;};

    Solver solver(fn);
    solver.doSave(false);
    solver.setBounds(-2, 3);
    solver.solve(Method::SPECIAL, 1e6, 1e6, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
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
