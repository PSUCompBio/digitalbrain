#include "FemTech.h"

#include <stdlib.h>

void StressUpdate(int e, int gp){
  FILE_LOG_SINGLE(DEBUGLOGIGNORE, "---- Calling Stress Update %d, %d ---", e, gp);
  switch (materialID[pid[e]]) {
    case 0 : // Rigid-body motion
             break;
    case 1 : //Compressible Neohookean
             CompressibleNeoHookean(e, gp);
             break;
    case 2 : // St. Venant-Kirchhoff
             StVenantKirchhoff(e, gp);
             break;
    case 3 : // Linear Elastic
             LinearElastic(e, gp);
             break;
    case 4 : // HGO with isotropic fiber distribution
             HGOIsotropic(e, gp);
             break;
    case 5 : // HGO with isotropic fiber distribution and viscoelasticity
             HGOIsotropicViscoelastic(e, gp);
             break;
    default : FILE_LOG_SINGLE(ERROR, "Unknown material type");
              TerminateFemTech(1);
  }
  return;
}
