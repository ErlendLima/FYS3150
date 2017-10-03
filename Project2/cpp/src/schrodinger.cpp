#include <armadillo>
#include "schrodinger.h"
#include "jacobi.h"

arma::mat hamiltonianMat(double rho_min, double rho_max, unsigned int N){
  double h = (rho_max - rho_min)/N;
  arma::vec V = arma::zeros<arma::vec>(N+1);
  for(unsigned int i = 0; i <= N; i++){
    V(i) = pow(rho_min + i*h, 2);           // Harmonic oscillator potential
  }
  double offdiag = -1.0/(h*h);
  arma::mat H = arma::mat(N+1, N+1, arma::fill::zeros);
  for (unsigned int row = 0; row < N+1; row++){
    if (row > 0)
      H(row, row-1) = offdiag;
    H(row,row) = 2/(h*h) + V(row);
    if (row < N)
      H(row, row+1) = offdiag;
  }
  return H;
}

arma::mat hamiltonianMat_repulsion(double rho_min, double rho_max, unsigned int N, double omega_r){
  double h = (rho_max - rho_min)/N;
  arma::vec V = arma::zeros<arma::vec>(N+1);
  double rho;
  for(unsigned int i = 0; i <= N; i++){
    rho = rho_min + i*h;
    V(i) = omega_r*omega_r*rho*rho + 1/rho; // Repulsive potential + harmonic oscillator
  }
  double offdiag = -1.0/(h*h);
  arma::mat H = arma::mat(N+1, N+1, arma::fill::zeros);
  for (unsigned int row = 0; row < N+1; row++){
    if (row > 0)
      H(row, row-1) = offdiag;
    H(row,row) = 2/(h*h) + V(row);
    if (row < N)
      H(row, row+1) = offdiag;
  }
  return H;
}

double solve(double rho_min, double rho_max, unsigned int N){
  // Setup matrix for task
  arma::mat H = hamiltonianMat(rho_min, rho_max, N);
  // Time and run algo
  auto start    = std::chrono::system_clock::now(); // Wall time
  std::clock_t startCPU = std::clock();                          // CPU clock start

  arma::vec eigval;
  arma::mat eigvec;
  jacobi(eigval, eigvec, H);
  // arma::eig_sym(eigval, eigvec, H);

  auto end = std::chrono::system_clock::now();
  std::clock_t endCPU = std::clock();
  double elapsed_seconds_CPU = static_cast<double>(endCPU - startCPU)/CLOCKS_PER_SEC;
  std::chrono::duration<double> elapsed_seconds = end-start;
  // std::cout << "Elapsed wall time: " << elapsed_seconds.count() << "s" << std::endl;
  // std::cout << "Elapsed CPU time: " << elapsed_seconds_CPU << "s" << std::endl;
  return elapsed_seconds_CPU;
}
