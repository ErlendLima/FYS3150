#ifndef ISING_H
#define ISING_H

#include <armadillo>
#include <map>
#include "metamodel.h"

int                  magnetization(const arma::imat& A);
arma::imat           setInitialStateRandom(unsigned int N);
arma::imat           setInitialStateOrdered(unsigned int N, int fill);
unsigned int         period(int x, unsigned int N);
int                  sumNeighbors(unsigned int i, unsigned int j, const arma::imat& A);
int                  totalEnergy(const arma::imat& A);
std::map<int,double> makeProbabilities(double beta);
void                 ising(const Metamodel& model);
void                 isingParallel(std::vector<double>& expectationValues,
                                   const Metamodel& model);

#endif /* ISING_H */
