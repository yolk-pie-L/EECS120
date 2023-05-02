/**
 *  \file mandelbrot_cyclic.cc
 *
 *  \brief Implement your parallel mandelbrot set in this file.
 */

#include <iostream>
#include <cstdlib>

int
main (int argc, char* argv[])
{
  double minX = -2.1;
  double maxX = 0.7; // x stands for real axis, y stands for imaginary axis
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

  double it = (maxX - minX)/width;
  double jt = (maxY - minY)/height;
  double x, y;


  gil::rgb8_image_t img(height, width);
  auto img_view = gil::view(img);

  y = minY;
  for (int j = 0; j < height; ++j) {
    x = minX;
    for (int i = 0; i < width; ++i) {
      img_view(i, j) = render(mandelbrot(x, y)/512.0);
      x += it;
    }
    y += jt;
  }
  gil::png_write_view("mandelbrot.png", const_view(img));
}
