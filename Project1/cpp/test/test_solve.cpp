#include <gtest/gtest.h>
#include <armadillo>
#include "../src/solver.h"

#define TOLERANCE 1e-6
TEST(LU, EulerCauchy){
    // Function to solve for; Euler-Cauchy equation
    double (*fn)(double) = [](double x){return 1.0;};
    // Solution for Dirichlet boundary conditions
    double (*u)(double) = [](double x){return -0.5*(x-1)*x;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::LU, 10, 10, 10);
    auto expected = solver.getDomain();
    expected.print();
    expected.transform(u);
    auto solution = solver.getSolution();
    solution.print();
    expected.print();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(General, EulerCauchy){
    // Function to solve for; Euler-Cauchy equation
    double (*fn)(double) = [](double x){return 1.0;};
    // Solution for Dirichlet boundary conditions
    double (*u)(double) = [](double x){return -0.5*(x-1)*x;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::GENERAL, 10, 10, 10);
    auto expected = solver.getDomain();
    expected.transform(u);
    auto solution = solver.getSolution();
    ASSERT_TRUE(arma::approx_equal(solution, expected, "reldiff", TOLERANCE));
}

TEST(Special, EulerCauchy){
    // Function to solve for; Euler-Cauchy equation
    double (*fn)(double) = [](double x){return 1.0;};
    // Solution for Dirichlet boundary conditions
    double (*u)(double) = [](double x){return -0.5*(x-1)*x;};

    Solver solver(fn);
    solver.doSave(false);
    solver.solve(Method::SPECIAL, 10, 10, 10);
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

    solver.solve(Method::GENERAL, 10, 10, 10);
    auto general = solver.getSolution();
    solver.solve(Method::SPECIAL, 10, 10, 10);
    auto special = solver.getSolution();
    solver.solve(Method::LU, 10, 10, 10);
    auto lu = solver.getSolution();

    bool generalNspecial = arma::approx_equal(general, special, "reldiff", TOLERANCE);
    bool specialNLU = arma::approx_equal(special, lu, "reldiff", TOLERANCE);
    ASSERT_TRUE(generalNspecial);
    ASSERT_TRUE(specialNLU);
}
