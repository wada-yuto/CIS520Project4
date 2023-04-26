#!/bin/bash -l
#SBATCH --mem=120G
#SBATCH --time=0:01:00
#SBATCH --job-name=Proj4
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --constraint=elves     # only run on elves
module load OpenMPI

mpirun --mca pml ob1 /homes/yuto808263/CIS520Project4/3way-mpi/src/MPI 