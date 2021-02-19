#include "FemTech.h"
#include "blas.h"

// St. Venant-Kirchhoff
// Evaluates the PK2 stress tensor

void StVenantKirchhoff(int e, int gp) {
  if (ndim == 2) {
    // 6 values saved per gauss point for 3d
    for (int i = 0; i < 3; i++) {
      int index = pk2ptr[e] + 3 * gp + i;
    }
  }
  if (ndim == 3) {
    int index = fptr[e] + ndim * ndim * gp;
    int pide = pid[e];
    double mu = properties[MAXMATPARAMS * pide + 1];    
    double lambda = properties[MAXMATPARAMS * pide + 2];

    // Compute Green-Lagrange Tensor: E= (1/2)*(F^T*F - I)
    // Compute Green-Lagrange Tensor: E= (1/2)*(H + H^T + H^T*H)
    double *H = mat1;
    ComputeH(e, gp, H);
    double *E = mat2;
    // Compute 0.5*(H+H^T)
    for (int i = 0; i < ndim; ++i) {
      for (int j = 0; j < ndim; ++j) {
        const int indexL = j + i*ndim;
        E[indexL] = 0.5*(H[indexL]+H[i+j*ndim]);
      }
    }
    // Compute 0.5*H^T H and add to E
    double half = 0.5;
    dgemm_(chy, chn, &ndim, &ndim, &ndim, &half, H, &ndim,
           H, &ndim, &one, E, &ndim);

    // Compute 2nd Piola-Kirchhoff Stress
    // S = lambda*tr(E)*I+2*mu*E
    double traceE = E[0] + E[4] + E[8];
    double *S = mat3;
    for (int i = 0; i < ndim2; ++i) {
      S[i] = 2.0 * mu * E[i];
    }
    S[0] += lambda * traceE;
    S[4] += lambda * traceE;
    S[8] += lambda * traceE;
    
    // Store F in H
    // F = H + I
    H[0] = H[0] + 1.0;
    H[4] = H[4] + 1.0;
    H[8] = H[8] + 1.0;

    double *sigmaTemp = mat4;
    double *sigma = mat2;
    double Jinv = 1.0/det3x3Matrix(H);
    // Compute F S F^T
    dgemm_(chn, chn, &ndim, &ndim, &ndim, &one, H, &ndim,
           S, &ndim, &zero, sigmaTemp, &ndim);
    dgemm_(chn, chy, &ndim, &ndim, &ndim, &Jinv, sigmaTemp, &ndim,
           H, &ndim, &zero, sigma, &ndim);

    // 6 values saved per gauss point for 3d
    // in voigt notation, sigma11
    sigma_n[0] = sigma[0];
    // in voigt notation, sigma22
    sigma_n[1] = sigma[4];
    // in voigt notation, sigma33
    sigma_n[2] = sigma[8];
    // in voigt notation, sigma23
    sigma_n[3] = sigma[7];
    // in voigt notation, sigma13
    sigma_n[4] = sigma[6];
    // in voigt notation, sigma12
    sigma_n[5] = sigma[3];
  } // if ndim == 3
  return;
}
