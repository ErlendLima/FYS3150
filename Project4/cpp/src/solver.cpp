#include <vector>
#include <mpi.h>
#include <fstream>
#include "solver.h"
#include "metamodel.h"
#include "ising.h"

void solve_sys(){
    // Sets up paralellization and repeatedly calls the ising - metropolis algorithm
    Metamodel model = Metamodel(); // Fetch model struct with parameters
    model.parallel = true;
    std::ofstream outstream;

    // Fetch from model because writing model.N everywhere looks ugly
    unsigned int N = model.N;
    unsigned int M = model.M;

    // Setup MPI. Init should take argv and argc, but this seems stupid. TODO: Look at this
    MPI_Init(NULL, NULL);

    // Fetch MPI parameters
    int NProcesses, RankProcess;
    MPI_Comm_size (MPI_COMM_WORLD, &NProcesses);
    MPI_Comm_rank (MPI_COMM_WORLD, &RankProcess);

    // Broadcast parameters to processes.
    MPI_Bcast (&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (&model.Tstart, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast (&model.Tstop,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast (&model.Tstep,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(RankProcess == 0){
        outstream.open(model.basepath + model.solverpath);
        // Write the header
        outstream << "Temperature E ESquared varE Cv M MSquared Mabs varM sus\n";
    }

    // Run MC Sampling by looping over temperatures
    for(double T = model.Tstart; T <= model.Tstop; T += model.Tstep){
        std::vector<double> LocalExpectationValues = {0.0, 0.0, 0.0, 0.0, 0.0};
        model.setTemperature(T);
        isingParallel(LocalExpectationValues, model);
        std::vector<double>TotExpectationValues = {0.0, 0.0, 0.0, 0.0, 0.0};
        for(int i = 0; i < 5; i++){
            MPI_Reduce(&LocalExpectationValues[i], &TotExpectationValues[i], 1,
                       MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        }

        if(RankProcess == 0){
            model.saveExpectationValues(outstream, TotExpectationValues, T, NProcesses);
            std::cout << "Done with t = " << T << '\n';
        }
    }

    if(RankProcess == 0)
        outstream.close();

  MPI_Finalize();
  model.write();
}
