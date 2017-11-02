#ifndef ISING_H
#define ISING_H

#include <armadillo>

int magnetization(arma::imat& A);
arma::imat setInitialStateRandom(unsigned int N);
arma::imat setInitialStateOrdered(unsigned int N, int fill);
unsigned int period(int x, unsigned int N);
int sumNeighbors(unsigned int i, unsigned int j, arma::imat& A);
int totalEnergy(arma::imat& A);
std::map<int,double> makeProbabilities(double beta);
void ising();

#endif /* ISING_H */
