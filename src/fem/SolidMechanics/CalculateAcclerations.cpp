#include "FemTech.h"
#include "blas.h"

void CalculateAccelerations(){
  if (debug && 1 == 0) {
    printf("RHS of solution\n");
    for (int i = 0; i < ndim*nnodes; ++i) {
	    printf("%d  %12.6f\n", i, f_net[i]);
	  }
  }
  for (int i = 0; i < ndim*nnodes; ++i) {
    accelerations[i] = f_net[i]/mass[i];
  }
	return;
}
