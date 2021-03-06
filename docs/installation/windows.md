---
description: >-
  These are the old instructions - for when we used Visual Studio... read
  below...
---

# Windows - Visual Studio \(OLD\)

Previously, the build system attempted to use Visual Studio. As we added dependencies, for example, BLAS/LAPACK, it was becoming more and more tricky. One reason was because BLAS/LAPACK needed Fortran compilers which is available for Cygwin for free, but for it to work with Visual Studio we would need to buy Intel compilers \(which we did not want to do\). So we switched to a "Cygwin-based build system". This is more similar to process on Mac and Linux. For the latest build instructions see the Cygwin-based build page.

This walks you through installation of Visual Studio \(VS\), CMake and Microsoft MPI \(MS-MPI\).

## CMake

* CMake-GUI:  [https://cmake.org/download](https://cmake.org/download/)

## Visual Studio

 Visual Studio 2017. We use the free community version.

## Microsoft MPI

You want to download the Microsoft MPI libraries. A good place to start is here: [https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi)  
Here we see there is a version 10:

![](../.gitbook/assets/ms-mpi-1.PNG)

![](../.gitbook/assets/ms-mpi-2.PNG)

It is **CRITICAL** that you download both the SDK and the MPI setup.exe. **YOU NEED BOTH!!!**

![](../.gitbook/assets/ms-mpi-3.PNG)

The next **CRITICAL** thing to do is **restart your computer** so your system environment variables get set. I don't believe there is another way around this if you don't admin privileges \(as I did not\).

## Paraview for Visualization

Download from: [https://www.paraview.org](https://www.paraview.org/)

