#!/bin/bash
#SBATCH --job-name=matrixMultiply
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=adam.jacobs@ufl.edu
#SBATCH --account=eel6763
#SBATCH --qos=eel6763
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=1000mb
#SBATCH -t 00:01:00
#SBATCH -o output

srun --mpi=$HPC_PMIX ./monte_carlo -10 10 5000
