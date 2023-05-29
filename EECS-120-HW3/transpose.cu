#include <stdlib.h>
#include <stdio.h>

#include "cuda_utils.h"
#include "timer.c"

typedef float dtype;

__global__ 
void copy(dtype* AT, dtype* A, int N)  {
	/* Fill your code here */
	int index = blockIdx.x * blockDim.x * blockDim.y  + threadIdx.x;
	int gridSize = gridDim.x * gridDim.y * blockDim.x * blockDim.y;
	for(; index < N * N; index += gridSize){
		AT[index] = A[index];
	}

}

__global__ 
void matTrans(dtype* AT, dtype* A, int N)  {
	/* Fill your code here */
  const int TILE_DIM = 64;
  int BLOCK_ROWS = 4;
  int x = blockIdx.x * TILE_DIM + threadIdx.x;
  int y = blockIdx.y * TILE_DIM + threadIdx.y;
  int width = gridDim.x * TILE_DIM;

  for (int j = 0; j < TILE_DIM; j+= BLOCK_ROWS)
    AT[x*width + (y+j)] = A[(y+j)*width + x];

}

__global__ 
void matTrans2(dtype* AT, dtype* A, int N)  {
	/* Fill your code here */
  const int TILE_DIM = 64;
  int BLOCK_ROWS = 4;
  __shared__ dtype tile[TILE_DIM][TILE_DIM + 1];
    
  int x = blockIdx.x * TILE_DIM + threadIdx.x;
  int y = blockIdx.y * TILE_DIM + threadIdx.y;
  int width = gridDim.x * TILE_DIM;

  for (int j = 0; j < TILE_DIM; j += BLOCK_ROWS)
     tile[threadIdx.x][threadIdx.y+ j] = A[(y+j)*width + x];

  __syncthreads();

  x = blockIdx.y * TILE_DIM + threadIdx.x;  // transpose block offset
  y = blockIdx.x * TILE_DIM + threadIdx.y;

  for (int j = 0; j < TILE_DIM; j += BLOCK_ROWS)
     AT[(y+j)*width + x] = tile[threadIdx.y+ j][threadIdx.x];
}

void
parseArg (int argc, char** argv, int* N)
{
	if(argc == 2) {
		*N = atoi (argv[1]);
		assert (*N > 0);
	} else {
		fprintf (stderr, "usage: %s <N>\n", argv[0]);
		exit (EXIT_FAILURE);
	}
}

void
initArr (dtype* in, int N)
{
	int i;

	for(i = 0; i < N; i++) {
		in[i] = (dtype) rand () / RAND_MAX;
	}
}

void
cpuTranspose (dtype* A, dtype* AT, int N)
{
	int i, j;

	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			AT[j * N + i] = A[i * N + j];
		}
	}
}

int
cmpArr (dtype* a, dtype* b, int N)
{
	int cnt, i;

	cnt = 0;
	for(i = 0; i < N; i++) {
		if(abs(a[i] - b[i]) > 1e-6) cnt++;
	}

	return cnt;
}

void
gpuTranspose (dtype* A, dtype* AT, int N)
{
	struct stopwatch_t* timer = NULL;
	long double t_gpu;  

	/* Setup timers */
	stopwatch_init ();
	timer = stopwatch_create ();

	dim3 gb(N/64, N/64, 1);
	dim3 tb(64, 4, 1);

	dtype* d_AT;
	dtype* d_A;
	CUDA_CHECK_ERROR (cudaMalloc (&d_AT, N * N * sizeof (dtype)));
	CUDA_CHECK_ERROR (cudaMalloc (&d_A, N * N * sizeof (dtype)));
	CUDA_CHECK_ERROR (cudaMemcpy (d_A, A, N * N * sizeof (dtype), 
				cudaMemcpyHostToDevice));
	/*warm up*/
	matTrans2 <<<gb, tb>>> (d_AT, d_A, N);

	stopwatch_start (timer);
	/* run your kernel here */
	matTrans2 <<<gb, tb>>> (d_AT, d_A, N);

	cudaDeviceSynchronize ();
	t_gpu = stopwatch_stop (timer);
	fprintf (stdout, "GPU transpose: %Lg secs ==> %Lg billion elements/second\n",
			t_gpu, (N * N) / t_gpu * 1e-9 );
	CUDA_CHECK_ERROR (cudaMemcpy (AT, d_AT, sizeof(dtype) * N * N, cudaMemcpyDeviceToHost));

}

int 
main(int argc, char** argv)
{
	/* variables */
	dtype *A, *ATgpu, *ATcpu;
	int err;

	int N;

	struct stopwatch_t* timer = NULL;
	long double t_cpu;

	N = -1;
	parseArg (argc, argv, &N);

	/* input and output matrices on host */
	/* output */
	ATcpu = (dtype*) malloc (N * N * sizeof (dtype));
	ATgpu = (dtype*) malloc (N * N * sizeof (dtype));

	/* input */
	A = (dtype*) malloc (N * N * sizeof (dtype));

	initArr (A, N * N);

	/* GPU transpose kernel */
	gpuTranspose (A, ATgpu, N);

	/* Setup timers */
	stopwatch_init ();
	timer = stopwatch_create ();

	stopwatch_start (timer);
	/* compute reference array */
	cpuTranspose (A, ATcpu, N);
	t_cpu = stopwatch_stop (timer);
	fprintf (stdout, "Time to execute CPU transpose kernel: %Lg secs\n",
			t_cpu);

	/* check correctness */
	err = cmpArr (ATgpu, ATcpu, N * N);
	if(err) {
		fprintf (stderr, "Transpose failed: %d\n", err);
	} else {
		fprintf (stdout, "Transpose successful\n");
	}

	free (A);
	free (ATgpu);
	free (ATcpu);

	return 0;
}
