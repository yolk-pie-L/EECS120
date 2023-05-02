/**
 *  \file mandelbrot_block.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include "render.hh"


int
mandelbrot(double x, double y) {
  int maxit = 511;
  double cx = x;
  double cy = y;
  double newx, newy;

  int it = 0;
  for (it = 0; it < maxit && (x*x + y*y) < 4; ++it) {
    newx = x*x - y*y + cx;
    newy = 2*x*y + cy;
    x = newx;
    y = newy;
  }
  return it;
}

int
main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("I am rank=%d, size=%d\n", rank, size);
  
  double start_time, end_time, elapsed_time;
  start_time = MPI_Wtime();
  double minX = -2.1;
  double maxX = 0.7;
  double minY = -1.25;
  double maxY = 1.25;
  
  int height, width;
  if (argc == 3) {
    height = atoi (argv[1]);
    width = atoi (argv[2]);
    assert (height > 0 && width > 0);
  } else {
    fprintf (stderr, "usage: %s <height> <width>\n", argv[0]);
    fprintf (stderr, "where <height> and <width> are the dimensions of the image.\n");
    return -1;
  }

  double it = (maxX - minX)/width; // x stand for real numbers
  double jt = (maxY - minY)/height;
  double x, y;

  uint start_j = height * rank / size;
  uint stop_j = height * (rank + 1) / size;
  if(rank == size - 1){
    stop_j = height - 1;
  }

  double* send_buffer = (double*) malloc(sizeof(double) * (stop_j - start_j) * width);
  double* recv_buffer = (double*) malloc(sizeof(double) * height * width);
  int* recv_counts = (int*)malloc(sizeof(int) * size);
  int *displs = (int*) malloc(sizeof(int) * size);
  for(int i = 0; i < size; i++){
    recv_counts[i] = width * (height * (i + 1) / size - (height * (i) / size));
  }
  displs[0] = 0;
  for(int i = 1; i < size; i++){
    displs[i] = displs[i - 1] + recv_counts[i - 1];
  }


  y = minY + jt * start_j;
  for (int j = 0; j < stop_j - start_j; ++j) {
    x = minX;
    for (int i = 0; i < width; ++i) {
      int a = mandelbrot(x, y);
      send_buffer[j * height + i] = a / 512.0;
      x += it;
    }
    y += jt;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Gatherv(send_buffer, (stop_j - start_j) * width, MPI_DOUBLE, 
  recv_buffer, recv_counts, displs, MPI_DOUBLE, 
  0, MPI_COMM_WORLD);

  if(rank == 0){
    gil::rgb8_image_t img(height, width);
    auto img_view = gil::view(img);

    y = minY;
    for (int j = 0; j < height; ++j) {
      x = minX;
      for (int i = 0; i < width; ++i) {
        img_view(i, j) = render(recv_buffer[j * height + i]);
        x += it;
      }
      y += jt;
    }
    gil::png_write_view("mandelbrot_block.png", const_view(img));
  }
  free(send_buffer);
  free(recv_buffer);
  end_time = MPI_Wtime();
  printf("Total running time: %f seconds\n", elapsed_time);
  MPI_Finalize();
}  

