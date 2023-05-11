#!/bin/bash
#SBATCH --job-name=Mandelbrot80      	## Name of the job.
#SBATCH --output=mandelbrot80.out		## Output log file
#SBATCH --error=mandelbrot30000.err		## Error log file
#SBATCH -A class-eecs120     		## Account to charge
#SBATCH -p standard          		## Partition/queue name
#SBATCH --nodes=2         		## Number of nodes
#SBATCH --ntasks=80 			## Number of tasks (MPI processes)

# Module load boost
module load boost/1.78.0/gcc.11.2.0

# Module load MPI
module load mpich/4.0/intel.2022.2

make clean
make mandelbrot_serial
make mandelbrot_block
make mandelbrot_cyclic
make mandelbrot_dynamic

width=20000
height=20000

# # Run the program 
echo "20000"
# ./mandelbrot_serial $width $height

echo "block"
mpirun -np $SLURM_NTASKS ./mandelbrot_block $width $height

echo "cyclic"
mpirun -np $SLURM_NTASKS ./mandelbrot_cyclic $width $height 

echo "dynamic"
mpirun -np $SLURM_NTASKS ./mandelbrot_dynamic $width $height
