#ifndef SOLVE_H
#define SOLVE_H

#include <armadillo>

void solveGeneral(double (*func)(double), const unsigned int low, const unsigned int high, const unsigned int step);
void solveLU(double (*func)(double), const unsigned int low, const unsigned int high, const unsigned int step);
arma::mat tridiagonalMat(unsigned int size, double upper, double middle, double lower);
arma::vec thomas(const arma::vec& a, const arma::vec& b, const arma::vec& c, const arma::vec& d);


#endif /* SOLVE_H */
