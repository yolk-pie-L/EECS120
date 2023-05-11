/**
 *  \file mandelbrot_dynamic.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "render.hh"
#include <mpi.h>

#define DATA_TAG 1
#define TERMINATE_TAG 2

using namespace std;

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

void master(int rank, int size, int height, int width){
  double start_time, end_time, elapsed_time;
  start_time = MPI_Wtime();

  int j = 0;
  for(int count = 1; count < size; count++){
    // printf("initial send---\n");
    MPI_Send(&j, 1, MPI_INT, count, DATA_TAG, MPI_COMM_WORLD);
    j++;
  }

  int count_j = 0;
  // printf("allocate recv_buffer\n");
  double* recv_buffer = (double*)malloc(sizeof(double) * (width + 1));
  // printf("allocate result\n");
  double* result = (double*)malloc(sizeof(double) * height * width);
  // printf("succesfully allocate\n");
  MPI_Status status;

  while(count_j < height){
    // printf("recv---\n");
    MPI_Recv(recv_buffer, width + 1, MPI_DOUBLE, MPI_ANY_SOURCE, DATA_TAG, MPI_COMM_WORLD, &status);
    int src_rank = status.MPI_SOURCE;
    int received_j = recv_buffer[0];
    // printf("memcpy---\n");
    memcpy(result + received_j * width, recv_buffer + 1, sizeof(double) * width);
    
    count_j++;
    if(j < height){
      // printf("send---\n");
      MPI_Send(&j, 1, MPI_INT, src_rank, DATA_TAG, MPI_COMM_WORLD);
      j++;
    }
  }
  for(int count = 1; count < size; count++){
    int data = 0;
    MPI_Send(&data, 1, MPI_INT, count, TERMINATE_TAG, MPI_COMM_WORLD);
  }
  end_time = MPI_Wtime();
  elapsed_time = end_time - start_time;
  gil::rgb8_image_t img(height, width);
  auto img_view = gil::view(img);

  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      img_view(i, j) = render(result[j * width + i]);
    }
  }
  gil::png_write_view("mandelbrot_dynamic.png", const_view(img));

  printf("Total running time: %f seconds\n", elapsed_time); 

}

void slave(int rank, int size, int height, int width){
  MPI_Status status;
  double* send_buffer = (double*)malloc(sizeof(double) * (width + 1));
  int j;

  double minX = -2.1;
  double maxX = 0.7;
  double minY = -1.25;
  double maxY = 1.25;
  double it = (maxX - minX)/width; // x stand for real numbers
  double jt = (maxY - minY)/height;
  
  MPI_Recv(&j, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);


  while(status.MPI_TAG == DATA_TAG){
    send_buffer[0] = j;
    double y = minY + jt * j;
    double x = minX;
    for(int counter = 1; counter <= width; counter++){
      send_buffer[counter] = mandelbrot(x, y) / 512.0;
      x += it;
    }
    MPI_Send(send_buffer, width + 1, MPI_DOUBLE, 0, DATA_TAG, MPI_COMM_WORLD);
    MPI_Recv(&j, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  }
}

int
main (int argc, char* argv[])
{  
  int rank, size;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

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

  if(rank == 0){
    master(rank, size, height, width);
  }else{
    slave(rank, size, height, width);
  }
  
  MPI_Finalize();
}

/* eof */
