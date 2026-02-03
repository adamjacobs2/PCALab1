#!/bin/bash
#SBATCH --job-name=integral
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=adam.jacobs@ufl.edu
#SBATCH --account=eel6763
#SBATCH --qos=eel6763
#SBATCH --nodes=1
#SBATCH --ntasks=32
#SBATCH --ntasks-per-node=32
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=1000mb
#SBATCH -t 00:05:00
#SBATCH -o SendRecvR32N100000

srun --mpi=$HPC_PMIX ./monte_carlo -10 10 100000
