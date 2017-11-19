#include <vector>
#include <mpi.h>
#include <fstream>
#include "solver.h"
#include "metamodel.h"
#include "ising.h"

void solveSystem(Metamodel& model){
    // Simple wrapper that runs the serial or parallelized version
    if (model.parallel){
        std::cout << "Running parallel." << std::endl;
        solveSystemParallel(model);
    }
    else{
        std::cout << "Running serial." << std::endl;
        solveSystemSerial(model);
    }
}

void solveSystemSerial(Metamodel& model){
    model.setTemperature(model.Tstart);
    ising(model);
}

void solveSystemParallel(Metamodel& model){
    // Sets up paralellization and repeatedly calls the ising - metropolis algorithm
    std::ofstream outstream;
    int waitNSteps;

    // Fetch from model because writing model.N everywhere looks ugly
    unsigned int N = model.N;
    unsigned int M = model.M;

    // Setup MPI. Init should take argv and argc, but this seems stupid. TODO: Look at this
    MPI_Init(NULL, NULL);

    // Fetch MPI parameters
    int NProcesses, RankProcess;
    MPI_Comm_size (MPI_COMM_WORLD, &NProcesses);
    MPI_Comm_rank (MPI_COMM_WORLD, &RankProcess);

    if(RankProcess == 0){
      outstream.open(model.basepath + model.solverpath);
      // Write the header
      outstream << "T E ESquared varE Cv M MSquared Mabs varM sus\n";
    }

    // Broadcast parameters to processes.
    MPI_Bcast (&M,            1, MPI_INT,    0, MPI_COMM_WORLD);
    MPI_Bcast (&N,            1, MPI_INT,    0, MPI_COMM_WORLD);
    MPI_Bcast (&model.Tstart, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast (&model.Tstop,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast (&model.Tstep,  1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // The seed must be different for each rank.
    model.seed += RankProcess;
    waitNSteps = 5000;

    double timeStart = MPI_Wtime();
    double timeSinceLast = timeStart;

    // Run MC Sampling by looping over temperatures
    for(double T = model.Tstart; T <= model.Tstop; T += model.Tstep){
        std::vector<double> LocalExpectationValues = {0.0, 0.0, 0.0, 0.0, 0.0};
        model.setTemperature(T);
        isingParallel(LocalExpectationValues, model, waitNSteps);
        std::vector<double>TotExpectationValues = {0.0, 0.0, 0.0, 0.0, 0.0};
        for(int i = 0; i < 5; i++){
            MPI_Reduce(&LocalExpectationValues[i], &TotExpectationValues[i],
                       1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        }
        if(RankProcess == 0){
            std::cout << "T: " << T << " [" << TotExpectationValues[0] << ", "
                      << TotExpectationValues[1] << ", "
                      << TotExpectationValues[2] << ", "
                      << TotExpectationValues[3] << "]" << std::endl;
            model.saveExpectationValues(outstream, TotExpectationValues, T, NProcesses, waitNSteps);
            std::cout << "Done for T = " << T << " in " << double(MPI_Wtime() - timeSinceLast) << " s" << std::endl;
            timeSinceLast = MPI_Wtime();
        }
    }

    if(RankProcess == 0){
        outstream.close();
        std::cout << "Done after: " << double(MPI_Wtime() - timeStart) << "s" << std::endl;
    }
  MPI_Finalize();
  model.write();
}
