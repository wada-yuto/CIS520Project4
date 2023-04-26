#!/bin/bash -l
##$ -l h_rt=0:01:00		# ask for 1 hour runtime
#SBATCH --constraint=moles
for i in 1 2
do
	 sbatch --constraint=mole --ntasks-per-node=$i --nodes=1 --mem-per-cpu=$((16/$i))G smallBatch.sh
done