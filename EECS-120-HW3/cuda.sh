#!/bin/bash
#SBATCH --job-name=reduce	      	## Name of the job.
#SBATCH --output=reduce.out		## Output log file
#SBATCH --error=reduce.err		## Error log file
#SBATCH -A class-eecs120    		## Account to charge
#SBATCH -p free-gpu              	## run on the gpu partition
#SBATCH -N 1                		## run on a single node
#SBATCH -n 1                		## request 1 task
#SBATCH -t 00:15:00         		## 15-minute run time limit
#SBATCH --gres=gpu:V100:1   		## request 1 gpu of type V100

# Module load Cuda Compiler
module load cuda/11.7.1

rm naive
rm stride
rm sequential

# Compile naive
nvcc naive.cu timer.c -o naive
nvcc stride.cu timer.c -o stride
nvcc sequential.cu timer.c -o sequential

# Runs a bunch of standard command-line
# utilities, just as an example:

echo "Script began:" `date`
echo "Node:" `hostname`
echo "Current directory: ${PWD}"

echo "naive"
./naive

echo "stride"
./stride

echo "sequential"
./sequential

echo ""
echo "=== Done! ==="

# eof
