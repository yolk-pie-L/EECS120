#!/bin/bash
#SBATCH --job-name=Mandelbrot      	## Name of the job.
#SBATCH --output=mandelbrot.out		## Output log file
#SBATCH --error=mandelbrot.err		## Error log file
#SBATCH -A class-eecs120     		## Account to charge
#SBATCH -p standard          		## Partition/queue name
#SBATCH --nodes=4          		## Number of nodes
#SBATCH --ntasks=160  			## Number of tasks (MPI processes)

# Module load boost
module load boost/1.78.0/gcc.11.2.0

# Module load MPI
module load mpich/4.0/intel.2022.2

make clean
make mandelbrot_serial
make mandelbrot_block
make mandelbrot_cyclic
make mandelbrot_dynamic

width=40000
height=40000

# # Run the program 
echo "serial"
./mandelbrot_serial $width $height > serial.txt

echo "block"
mpirun -np $SLURM_NTASKS ./mandelbrot_block $width $height > block.txt 

echo "cyclic"
mpirun -np $SLURM_NTASKS ./mandelbrot_cyclic $width $height > cyclic.txt 

echo "dynamic"
mpirun -np $SLURM_NTASKS ./mandelbrot_dynamic $width $height > dynamic.txt 