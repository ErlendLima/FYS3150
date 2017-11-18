#ifndef SOLVER_H
#define SOLVER_H

#include "metamodel.h"

void solveSystem(Metamodel&);
void solveSystemParallel(Metamodel&);
void solveSystemSerial(Metamodel&);

#endif /* SOLVER_H */
