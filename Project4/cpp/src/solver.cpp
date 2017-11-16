#include <vector>
#include <mpi.h>
#include "solver.h"
#include "metamodel.h"
#include "ising.h"

void solve_sys(){
  // Sets up paralellization and repeatedly calls the ising - metropolis algorithm
  // to aquire as much data as possible.
  metamodel model;

  unsigned int N = model.N;
  unsigned int M = model.M;

  std::vector<double> expectationValues = {0.0, 0.0, 0.0, 0.0, 0.0};

  // Setup MPI. Init should take argv and argc, but this seems stupid. TODO: Look at this
  MPI_Init(NULL, NULL);
  // PARALELLIZED:
  // for temperature in temperatures:
  //   ising()
  int NProcesses, RankProcess;
  MPI_Comm_size (MPI_COMM_WORLD, &NProcesses);
  MPI_Comm_rank (MPI_COMM_WORLD, &RankProcess);

  // Broadcast parameters to processes.
  MPI_Bcast (&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast (&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast (&model.Tstart, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast (&model.Tstop,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast (&model.Tstep,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Run MC Sampling by looping over temperatures
  for(double T = model.Tstart; T <= model.Tstop; T += model.Tstep){
    std::vector<double> LocalExpectationValues = {0.0, 0.0, 0.0, 0.0, 0.0};
    isingParallel(T, LocalExpectationValues, model);
    std::vector<double> TotExpectationValues = {0.0, 0.0, 0.0, 0.0, 0.0};
    for(int i = 0; i < 5; i++){
      MPI_Reduce(&LocalExpectationValues[i], &TotExpectationValues[i], 1,
                 MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();
}
