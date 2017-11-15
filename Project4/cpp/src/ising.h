#ifndef ISING_H
#define ISING_H

#include <armadillo>
#include <json/json.h>
#include <json/writer.h>
#include "metamodel.h"

int                  magnetization(arma::imat& A);
arma::imat           setInitialStateRandom(unsigned int N);
arma::imat           setInitialStateOrdered(unsigned int N, int fill);
unsigned int         period(int x, unsigned int N);
int                  sumNeighbors(unsigned int i, unsigned int j, arma::imat& A);
int                  totalEnergy(arma::imat& A);
std::map<int,double> makeProbabilities(double beta);
void                 ising();
struct               parameters;

#endif /* ISING_H */
