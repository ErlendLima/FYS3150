#ifndef SOLVE_H
#define SOLVE_H

#include <armadillo>

arma::mat tridiagonalMat(unsigned int size, double upper, double middle, double lower);
arma::vec thomas(const arma::vec& a, const arma::vec& b, const arma::vec& c, const arma::vec& d);
arma::vec thomasSpecial(const arma::vec& f);


#endif /* SOLVE_H */
