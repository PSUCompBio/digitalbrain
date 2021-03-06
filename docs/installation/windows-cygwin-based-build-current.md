---
description: Explains how to build using Cygwin...
---

# Windows - Cygwin Based Build \(Current\)

The current suggested setup to work on Windows is to use Atom \(or Vi for advanced linux users\) for editing source code and Cygwin terminal for building \(a.k.a compiling\). 

## Step-by-step Instructions

1. Download and install Atom: [https://atom.io/](https://atom.io/)
2. Download and install Cygwin: [https://cygwin.com/](https://cygwin.com/). Use Cygwin installer to install the following packages:
   1. vim 8.0
   2. vim-common 8.0
   3. git 2.17.0-1
   4. make 4.2.1-2
   5. cmake 3.6.2-1
   6. gcc-core 7.4
   7. gcc-fortran 7.4
   8. gcc-g++ 7.4
   9. libopenmpi12 1.10.7-1        
   10. libopenmpi-devel 3.1.1-2  
   11. openmpi 3.1.2-2 
   12. liblapack-devel 3.8.0-1
   13. libopenblas 0.3.5-1 
   14. gnuplot 5.2.6-2
   15. lua 5.3.5-1 
   16. lua-devel 5.3.5.1
   17. xinit 1.3.4-14
   18. liblmlib2-devel 
   19. libGLEW-devel
   20. libXrender-devel
   21. libXrandr-devel
   22. libhwloc-devel
   23. libevent-devel
3. Download and install Paraview: [https://www.paraview.org/](https://www.paraview.org/)
4. **Build FemTech**:  - Open cygwin terminal - Navigate to a directory where you would like to install FemTech \(e.g.  mkdir code/FemTech\) - cd code - git clone [https://github.com/PSUCompBio/FemTech](https://github.com/PSUCompBio/FemTech) - cd FemTech - mkdir build - cd build - ccmake ..  - In the ccmake GUI, add "Enable MPI" and "Examples", then configure \(using the "c" button\). If there are no errors, go to next step. If you get error see **Common Build Errors** below.    - If you have no configuration errors, then in the ccmake GUI, you should see a list of examples. Select the examples you would like to turn on \(see list here\), and configure again.  - If you have no configuration errors, then select generate \(g\) to create the Makefile. make -j 8 \(where 8 is the number of processors you have\)

## Common Build Errors

### CMake Error: Could NOT find MPI\_CXX \(missing: MPI\_CXX\_LIBRARIES\)

It is same as MPI\_C\_LIBRARIES so just copy and paste /usr/lib/libmpi.dll.a into MPI\_CXX\_LIBRARIES

