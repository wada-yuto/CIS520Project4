#!/bin/bash -l
##$ -l h_rt=0:01:00		# ask for 1 hour runtime
#SBATCH --constraint=elves     # only run on elves
module load OpenMPI

mpirun --mca pml ob1 /homes/yuto808263/CIS520Project4/3way-mpi/src/MPI