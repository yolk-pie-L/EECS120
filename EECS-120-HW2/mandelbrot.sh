#!/bin/bash
#SBATCH --job-name=Mandelbrot      	## Name of the job.
#SBATCH --output=mandelbrot.out		## Output log file
#SBATCH --error=mandelbrot.err		## Error log file
#SBATCH -A class-eecs120     		## Account to charge
#SBATCH -p standard          		## Partition/queue name
#SBATCH --nodes=1            		## Number of nodes
#SBATCH --ntasks=1  			## Number of tasks (MPI processes)

# Module load boost
module load boost/1.78.0/gcc.11.2.0

# Module load MPI
module load mpich/4.0/intel.2022.2

# Run the program 
mpirun -np $SLURM_NTASKS ./mandelbrot_serial 1000 1000
