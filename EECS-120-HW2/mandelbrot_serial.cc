/**
 *  \file mandelbrot_serial.cc
 *  \brief HW 2: Mandelbrot set serial code
 */


#include <iostream>
#include <cstdlib>
#include <chrono>
#include "render.hh"
#include <stdio.h>

using namespace std;

#define WIDTH 1000
#define HEIGHT 1000

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
  auto start_time = std::chrono::high_resolution_clock::now();
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

  printf("width: %d height: %d\n", width, height);

  double it = (maxX - minX)/width; // x stand for real numbers
  double jt = (maxY - minY)/height;
  double x, y;


  gil::rgb8_image_t img(height, width);
  auto img_view = gil::view(img);

  // FILE* fp;
  // fp = fopen("serial.txt", "w");
  double* res = (double*) malloc(sizeof(double) * height * width);
  y = minY;
  for (int j = 0; j < height; ++j) {
    x = minX;
    for (int i = 0; i < width; ++i) {
      res[j * width + i] = mandelbrot(x, y)/512.0;
      x += it;
    }
    y += jt;
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  y = minY;
  for (int j = 0; j < height; ++j) {
    x = minX;
    for (int i = 0; i < width; ++i) {
      img_view(i, j) = render(res[j* width + i]);
      x += it;
    }
    y += jt;
  }

  gil::png_write_view("mandelbrot_serial.png", const_view(img));

  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
  cout << "Total running time: " << duration_ms / 1000.0 << " seconds" << endl;

}

/* eof */
