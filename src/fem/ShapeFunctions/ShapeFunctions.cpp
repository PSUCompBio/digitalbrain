#include "FemTech.h"
#include "blas.h"

/* Global Variables */
int *gptr;
// TODO(anil) dsptr[i] = ndim*gptr[i] always. Could be eliminated.
int *dsptr;
int *GaussPoints;
double *shp;
double *dshp; //pointer for deriviatives of shp functions
int *fptr; //pointer for deformation gradient, F
int *nShapeFunctions;
double *F; // deformation gradient array, F

int *gpPtr;
double *detJacobian;
double *gaussWeights;

void ShapeFunctions() {
  // Global Array - keeps track of how many gauss points there are
  // per element.
  GaussPoints = (int *)malloc(nelements * sizeof(int));
  // Global Array - keeps track of how many shp functions there are
  // per element.
  nShapeFunctions = (int *)malloc(nelements * sizeof(int));

  // Global array -
  // When number of shape functions in an element equals
  // the number of nodes in an element, gptr array looks
  // similar to eptr array. However, the number of quadrature
  // points (a.k.a gauss points) can be different. For example,
  // for a 8-noded hex, there can be 8 gauss points or there
  // could be 1. The gptr array works like the eptr array, but
  // allows this difference to occur.
  gptr = (int *)malloc((nelements+1) * sizeof(int));
  dsptr = (int *)malloc((nelements + 1) * sizeof(int));
  gpPtr = (int *)malloc((nelements+1) * sizeof(int));
  fptr = (int *)malloc((nelements+1) * sizeof(int));

  int counter = 0; //counter for storage of shp nShapeFunctions
  int dshp_counter = 0; // counter for deriviatives of shp function
  int gpCount = 0; // counter for gaupp points
	int F_counter = 0; //counter for storage of deformation gradient, F

  gptr[0] = 0;
  dsptr[0] = 0;
  gpPtr[0] = 0;
	fptr[0]=0;
  for (int i = 0; i < nelements; i++) {
    if (strcmp(ElementType[i], "C3D8") == 0) {
      //GuassPoints per element
      gpCount = 8;
      nShapeFunctions[i] = 8;

      // shp function array needs to hold 8
      // shp functions for each of these 8 gauss points
      // for this one element there are 8 gauss points,
      // which each have 8 components in shp function array
      // so for this element I need 8 * 8 positions to hold

      // counter = counter + nShapeFunctions[i]*GaussPoints[i];
      counter += 64;

      // the next counter is used to determine the size of the
      // derivative of shp function, dshp. We expand the slots
      // to account for ndim, since derivatives are taken with
      // respect to chi, eta, and iota.
      // dshp_counter = dshp_counter + (ndim * nShapeFunctions[i]*GaussPoints[i]);
      dshp_counter += 192;

      // the next counter is for the deformation gradient
			// since there is a deformation gradient stored at each
      // gauss point. The deformation graient, F is normally
      // ndim*ndim, but since we are saving F for each gauss point
 			// it is ndim*ndim*ngausspoint =3*3*8 = 72. Also, because we can have mixed meshes,
      // we need a way to reference F as well. (An Fptr)
			F_counter += 72;
    }
    if (strcmp(ElementType[i], "C3D4") == 0) {
      gpCount = 1;
      nShapeFunctions[i] = 4;
      // same argument as above
      // counter = counter + nShapeFunctions[i]*GaussPoints[i];
      // dshp_counter = dshp_counter + (ndim * nShapeFunctions[i]*GaussPoints[i]);
			// F_counter = ndim*ndim*ngausspoint = 3*3*1
      counter += 4;
      dshp_counter += 12;
			F_counter += 9;

      // gpCount = 4;
      // nShapeFunctions[i] = 4;
      // counter += 16;
      // dshp_counter += 48;
    }
    GaussPoints[i] = gpCount;
    gptr[i + 1] = counter;
    dsptr[i + 1] = dshp_counter;
    gpPtr[i + 1] = gpPtr[i]+gpCount;
    fptr[i+1]= F_counter;
  }

  // for debugging purposes
  if (debug && 1==0) {
    for (int i = 0; i < nelements; i++) {
      printf("(e.%d) - eptr:[%d->%d] - gptr:[%d->%d] -  dsptr:[%d->%d] - fptr:[%d->%d]\n",
 				i, eptr[i], eptr[i + 1], gptr[i], gptr[i + 1], dsptr[i], dsptr[i + 1],
				fptr[i],fptr[i + 1]);
    }
    printf("size of shp array = %d \n", counter);
    printf("size of derivatives of shp functions array, dshp = %d \n", dshp_counter);
		printf("size of deformation gradient array, F = %d \n", F_counter);
  }

  /*set size of shp array  - this holds shp functions for all elements */
  shp =  (double *)calloc(counter, sizeof(double));
  /*set size of dshp array  - this holds derivatives of shp functions for all elements */
  dshp = (double *)calloc(dshp_counter, sizeof(double));
  detJacobian = (double *)calloc(gpPtr[nelements], sizeof(double));
  gaussWeights = (double *)calloc(gpPtr[nelements], sizeof(double));
  /* set size of deformation gradient, F array -
		it holds F for all gauss points in all elemnts */
  F = (double *)calloc(F_counter, sizeof(double));

  /* for debugging */
  if (debug && 1==0) {
    for (int i = 0; i < nelements; i++) {
      printf("e.%d: int. pts = %d, # shp functions = %d\n", i, GaussPoints[i], nShapeFunctions[i]);
      for (int j = 0; j < GaussPoints[i]; j++) {
        for (int k = 0; k < nShapeFunctions[i]; k++) {
          printf(" %d", gptr[i] + j * GaussPoints[i] + k);
        }
        printf("\n");
      }
      printf("\n");
      for (int j = 0; j < GaussPoints[i]; j++) {
        for (int k = 0; k < nShapeFunctions[i]; k++) {
          for (int l = 0; l < ndim; l++) {
            printf("%d ", dsptr[i]  +  j*GaussPoints[i]*ndim  +  k*ndim + l);
          }
          printf("\n");
        }
        printf("\n");
      }
    }
  }

 // Depending on element type call correct shape function library
  for (int i = 0; i < nelements; i++) {
    // 3D 8-noded hex shape function routine
    if (strcmp(ElementType[i], "C3D8") == 0) {
      double *Chi = (double*)malloc(GaussPoints[i] * ndim * sizeof(double));
      double *GaussWeightsLocal = &(gaussWeights[gpPtr[i]]);
      double *detJacobianLocal = &(detJacobian[gpPtr[i]]);
      GaussQuadrature3D(i, GaussPoints[i], Chi, GaussWeightsLocal);
      for (int k = 0; k < GaussPoints[i]; k++) {
        ShapeFunction_C3D8(i, k, Chi, detJacobianLocal);
      }
      // Free all allocated memories
      free(Chi);
    }

    // 3D 4-noded tet shape function routine
    if (strcmp(ElementType[i], "C3D4") == 0) {
      double *Chi = (double*)malloc(GaussPoints[i]* ndim * sizeof(double));
      double *GaussWeightsLocal = &(gaussWeights[gpPtr[i]]);
      double *detJacobianLocal = &(detJacobian[gpPtr[i]]);
      GaussQuadrature3D(i, GaussPoints[i], Chi, GaussWeightsLocal);
      //printf("chi, eta, iota = %f, %f, %f\n", Chi[0], Chi[1], Chi[2]);
      for (int k = 0; k < GaussPoints[i]; k++) {
        ShapeFunction_C3D4(i, k, Chi, detJacobianLocal);
      }
      free(Chi);
      // free(GaussWeights);
    }
  }// loop on nelements

  // for debugging
  if (debug && 1==0) {
    for (int i = 0; i < nelements; i++) {
      printf("shp array e.%d with %d Gauss points, each with %d shp functions \n", i, GaussPoints[i], nShapeFunctions[i]);
      for (int j = 0; j < GaussPoints[i]; j++) {
        printf("int.%d:\n", j);
        for (int k = 0; k < nShapeFunctions[i]; k++) {
          //printf("%8.5f ", shp[gptr[i] + j * GaussPoints[i] + k]);
          printf(" shp: %4.4f dshp: %8.4f %8.4f %8.4f\n",
              shp[gptr[i] + j * GaussPoints[i] + k],
              dshp[dsptr[i] + j * GaussPoints[i] * ndim + k * ndim + 0],
              dshp[dsptr[i] + j * GaussPoints[i] * ndim + k * ndim + 1],
              dshp[dsptr[i] + j * GaussPoints[i] * ndim + k * ndim + 2]);
        }
        printf("\n");
      }
    }
  }
  return;
}
