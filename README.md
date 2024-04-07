# Lattice Boltzmann Method

LBM gas simulation is a project made for Discrete Modeling classes.

# Idea of the LBM

The Lattice Boltzmann Method (LBM) is a class of CFD methods. In contrast to classic CFD simulations based on solving Navier-Stokes equations, LBM calculates flow through a lattice with streaming and collision operations.

The domain is divided into squares. Fluid density is determined for each cell based on the flows from and to adjacent cells.

One of LBM's advantages is its efficiency on massively parallel architectures.

# Implementation

Regarding the characteristics of LBM, I decided to implement this algorithm using the CUDA framework for GPU programming. Visualization is implemented using OpenGL.
