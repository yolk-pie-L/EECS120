#!/bin/bash
#SBATCH --job-name=Mandelbrot      	## Name of the job.
#SBATCH --output=mandelbrot.out		## Output log file
#SBATCH --error=mandelbrot.err		## Error log file
#SBATCH -A class-eecs120     		## Account to charge
#SBATCH -p standard          		## Partition/queue name
#SBATCH --nodes=2            		## Number of nodes
#SBATCH --ntasks=30  			## Number of tasks (MPI processes)

# Module load boost
module load boost/1.78.0/gcc.11.2.0

# Module load MPI
module load mpich/4.0/intel.2022.2

make clean
make mandelbrot_serial
make mandelbrot_block

# Run the program 
echo "serial"
./mandelbrot_serial 5000 5000 

echo "block"
mpirun -np $SLURM_NTASKS ./mandelbrot_block 5000 5000 