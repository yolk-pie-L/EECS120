#!/bin/bash
#SBATCH --job-name=mergesort-omp      	## Name of the job.
#SBATCH --output=mergesort.out		## Output log file
#SBATCH --error=mergesort.err		## Error log file
#SBATCH -A class-eecs120      		## Account to charge
#SBATCH -p standard          		## Partition/queue name
#SBATCH --nodes=1            		## Request 1 node
#SBATCH --cpus-per-task=40 		## Number of threads per task (OMP threads)

# Module load intel compiler 
module load intel/2022.2

# NOTE: 
#Note that we define the value of OMP_NUM_THREADS to be equal to the value of 
#the SLURM defined environment variable $SLURM_CPUS_PER_TASK. By defining 
#OMP_NUM_THREADS in the this way we ensure that any change to the parameter 
#--cpus-per-task in our script will automatically be communicated to OMP.

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
# OMP_NUM_THREADS=10

make clean
make mergesort-seq
make mergesort-omp

# Run OpenMP mergesort on 10 million elements
echo mergesort-seq-100000000
./mergesort-seq 100000000


echo mergesort-omp-100000000
./mergesort-omp 100000000

