#include <iostream>
#include <random>
#include <armadillo>
#include <array>
#include <random>
#include <string>
#include <chrono>
#include <thread>
#include "ising.h"
#include "metamodel.h"

int magnetization(const arma::imat& A){
  // Return the magnetization of a state described by A, which is simply the sum
  // of the spins/elements. This function is somewhat redundant.
  return arma::accu(A);
}

arma::imat setInitialStateRandom(unsigned int N){
  // Generate armadillo integer matrix with uniform random negative or positive
  // ones.
  std::mt19937 gen;
  gen.seed(1233);
  std::uniform_real_distribution<double> dist(0.0,1.0);

  arma::imat state = arma::zeros<arma::imat>(N, N);
  for(unsigned int row = 0; row < N; row++){
    for(unsigned int col = 0; col < N; col++){
      if(dist(gen) < 0.5){
        state(row,col) = -1;
      }
      else{
        state(row,col) = 1;
      }
    }
  }
  return state;
}

arma::imat setInitialStateOrdered(unsigned int N, int fill){
  arma::imat state = arma::ones<arma::imat>(N, N);
  state.fill(fill);
  return state;
}

unsigned int period(int x, unsigned int N){
  unsigned int a = ((x+N)%N);
  if(a == 0){
    return 0;
  }
  else{
    return a;
  }
}

int sumNeighbors(unsigned int i, unsigned int j, const arma::imat& A){
  unsigned int N = A.n_cols;
  int s = 0;
  s += A(period(i-1, N), j)  // Left neighbor
    +  A(i, period(j+1, N))  // Lower neighbor
    +  A(i, period(j-1, N))  // Upper neighbor
    +  A(period(i+1, N), j); // Right neighbor
  return s;
}

int totalEnergy(const arma::imat& A){
  int E = 0;
  for(unsigned int i = 0; i < A.n_cols; i++){
    for(unsigned int j = 0; j < A.n_rows; j++){
      E -= A(i,j)*sumNeighbors(i,j,A);
    }
  }
  return E;
}

std::map<int,double> makeProbabilities(double beta){
  std::array<int, 5> dEStates{-4, -2, 0, 2, 4};
  std::map<int,double> probabilities;
  for(const auto& dE: dEStates){
    probabilities[dE] = exp(-beta*dE);
  }
  return probabilities;
}

void ising(const Metamodel& model){
  int seed = model.seed;
  // Setup RNG generator
  std::mt19937 gen;
  gen.seed(seed);
  std::uniform_real_distribution<double> random(0.0,1.0);

  // Setup and run simulation of the Ising model. Unpack some vals from model.
  unsigned int N = model.N;      // Lattice size (N x N)
  unsigned int M = model.M;         // Number of MC-iterations
  int m, n, dE;

  // Create probability map for possible energy changes
  auto probabilities = makeProbabilities(model.beta);

  // Initialize arrays and initial values
  arma::imat state = setInitialStateRandom(N);
  std::cout << sizeof(state(0,0)) << std::endl;
  std::vector<arma::imat> states (model.M);
  states[0] = state; // Saves every state

  std::vector<double> energies (model.M);         // Save energy for each MC Cycle
  double energy = totalEnergy(state);
  energies[0] = energy;

  std::vector<int> magmoments (model.M);          // Hold magnetization for each MC
  int magmoment = magnetization(state);
  magmoments[0] = magmoment;

  // Loop through MC-cycles
  unsigned int nSpinsTot = N*N;
  for(unsigned int i = 1; i < M; i++){
    for(unsigned int j = 0; j < nSpinsTot; j++){
        // Choose row (m) and column (n) index to perturb/flip given acceptance
        m = static_cast<int>(random(gen)*N);
        n = static_cast<int>(random(gen)*N);
        // Calculate consequent change of energy
        dE = 2*state(m,n)*sumNeighbors(m,n,state);
        // Metropolis algorithm
        if(dE < 0 || random(gen) <= probabilities[dE]){
        // if(random(gen) <= probabilities[dE]){
          state(m,n) *= -1;
          energy += dE;
          magmoment += 2*state(m,n);
        }
      }
      // Update states
      states[i] = state;
      energies[i] = energy;
      magmoments[i] = magmoment;
  }
  // Save all data to binary files
  model.save(states, energies, magmoments);
  // Write metadata to be read by Python before reading binary files
  model.write();
}

void isingParallel(double T, std::vector<double>& expectationValues, const Metamodel& model){
    const int seed = model.seed;
    // Setup RNG generator
    std::mt19937 gen;
    gen.seed(seed);
    std::uniform_real_distribution<double> random(0.0,1.0);

    // Setup and run simulation of the Ising model. Unpack some vals from model.
    unsigned int N = model.N;      // Lattice size (N×N)
    unsigned int M = model.M;      // Number of MC-iterations
    int m, n, dE;

    // Create probability map for possible energy changes
    const auto probabilities = makeProbabilities(model.beta);

    // Initialize values
    arma::imat state = setInitialStateRandom(N);
    double energy = totalEnergy(state);
    int magMoment = magnetization(state);

    // Loop through MC-cycles
    unsigned int numSpinsTot = N*N;
    for(unsigned int i = 1; i < M; i++){
        // Loop over the number of nodes in the lattice
        for(unsigned int j = 0; j < numSpinsTot; j++){
            // Choose row (m) and column (n) index to perturb/flip given acceptance
            m = static_cast<int>(random(gen)*N);
            n = static_cast<int>(random(gen)*N);
            // Calculate consequent change of energy
            dE = 2*state(m,n)*sumNeighbors(m,n,state);
            // Metropolis algorithm
            if(dE < 0 || random(gen) <= probabilities.at(dE)){
                state(m,n) *= -1;
                energy += dE;
                magMoment += 2*state(m,n);
            }
        }
    }

    // Add relevant values to total array
    expectationValues[0] += energy;
    expectationValues[1] += energy*energy;
    expectationValues[2] += magMoment;
    expectationValues[3] += magMoment*magMoment;
    expectationValues[4] += fabs(magMoment);
}
