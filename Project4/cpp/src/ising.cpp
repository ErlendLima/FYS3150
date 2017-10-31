#include "ising.h"

int magnetization(arma::mat& A){
  // Return the magnetization of a state described by A, which is simply the sum
  // of the spins.
  return arma::accu(A);
}
