#include <iostream>
#include <cstdlib>
#include <stdio.h>

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

int main(){
    double r = mandelbrot(-1.988000, 0.000000) / 512.0;
    printf("%f\n", r);
    double minY = -1.25;
    double maxY = 1.25;
    double jt = (maxY - minY) / 50;
    double a = minY + jt * 25;
    double b = minY;
    for(int i = 0; i < 25; i++){
      b += jt;
    }
    printf("a=%f b=%f\n", a, b);
    if(a == b){
      printf("equals a, b\n");
    }else{
      printf("notequals a, b\n");
    }
    if(a == 0.0){
      printf("a equals 0\n");
    }
    if(b == 0.0){
      printf("b equals 0");
    }
    printf("m(a)=%f\n", mandelbrot(-1.988000, a)/512.0);
    printf("m(b)=%f\n", mandelbrot(-1.988000, b)/512.0);
}