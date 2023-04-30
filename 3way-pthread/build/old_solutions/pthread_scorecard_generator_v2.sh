#!/bin/sh
#SBATCH --mem=120G
#SBATCH --time=24:00:00
#SBATCH --job-name=cis520pthreadscorecardgeneratorv2
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=10

echo "Running scorecard generator v2 on $HOSTNAME"

./pthread_scorecard_generator_v2 /homes/dan/625/wiki_dump.txt
# ./pthread_scorecard_generator_v2 ../wonderland.txt

echo "Finished run on $SLURM_NTASKS $HOSTNAME cores"

echo " "
