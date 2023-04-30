#!/bin/sh
#SBATCH --mem=120G
#SBATCH --time=24:00:00
#SBATCH --job-name=cis520pthreadscorecardgenerator
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2

echo "Running scorecard generator on $HOSTNAME"

./pthread_scorecard_generator_v3 /homes/dan/625/wiki_dump.txt
#./pthread_scorecard_generator_v3 ../wonderland.txt

echo "Finished run on $SLURM_NTASKS $HOSTNAME cores"

echo " "
