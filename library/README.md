# Project Overview

Interprocessing Communication Library (ICL)

The proeject is aimed at builidng a generic Inter Process Communication Library. This library is aimed at providing the user the flexibility of making threads based applications that require threads communicating with other threads within the application or with threads in the other application. The library provides:
1. Thread wrappers for managing thread commnications.
2. Functions to produce and consume messages.
3. A buffer management system for thread communication.
4. Easy exchange of messages.
5. Non blocking way of receiving messages.
6. Application supervision.
7. Thread notification system.

These features makes the use of library more integrated with an application and provides robust inter process communication.

# Target Build System
For this assignment, Buildroot will be used for building the application with for beaglebone.  
Target build systems include:

1. x86
2. ARM systems
3. PowerPC

And systems which supports C++11 threads and/or pthreads. 

# Hardware Platform
Tests will be performed on the following hardware:  
1. Beaglebone black with buildroot linux loaded with 2 applications performing interprocess communication using ICL. 
2. A baglebone and an application on x86 will be able to communicate over the same network.

Board borrowed for this project:
1. Beaglebone balck - 1

# Open Source Projects Used
1. Buildroot: Provides steps for compiling linux for target.
2. C/C++ : Language used for developement.
3. Linux : Target OS for the library.
4. Busybox : Used in conjunction with buildroot to populate rootfs.
5. GNU Makefile: helps in dependency based compilations.

# Previously Discussed Content
1. Assignment on IPC using sockets.
2. Buildroot in Assignments and Mastering Embedded Linux Programming.
3. pThreads from Assignments.

# New Content
1. Design orientation.
2. Process supervision.
3. Easy use of intra process and inter process communication.
4. 
# Shared Material
# Source Code Organization
# Schedule Page