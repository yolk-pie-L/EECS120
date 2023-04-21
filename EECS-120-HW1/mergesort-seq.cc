/**
 *  \file mergesort.cc
 *
 *  \brief Implement your mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <omp.h>
#include "sort.hh"

int psize = 50000;

inline void simple_sort(int N, keytype* A, keytype* res_arr){
  if(N == 1){
    *res_arr = *A;
    return;
  }
  keytype a = *A;
  keytype b = *(A + 1);
  if(a > b){
    *res_arr = b;
    *(res_arr + 1) = a;
  }else{
    *res_arr = a;
    *(res_arr + 1) = b;
  }
}

void
mySort (int N, keytype* A)
{
  keytype* A_copy = (keytype*)malloc(N * sizeof(keytype));
  memcpy(A_copy, A, sizeof(keytype) * N);
  #ifdef DEBUG
  printf("\n\n start mysort\nfinish copy A to A_copy\n");
  #endif
  #pragma omp parallel
  {
    #pragma omp single
    {
    pmergesort(N, A_copy, A);
    printf("num_threads=%d\n", omp_get_num_threads());
    printf("psize=%d\n", psize);
    }
  }
}

void pmergesort(int N, keytype* A, keytype* res_arr){
  if(N <= 2){
    simple_sort(N, A, res_arr);
    return;
  }
  int half_length = N / 2;
  if (N > psize){
    #pragma omp task shared(res_arr, A) 
    {
    pmergesort(half_length, res_arr, A);
    }
    #ifdef DEBUG
    printf("pmergesort arr1: ");
    print_arr(A, half_length);
    #endif
    #pragma omp task shared(res_arr, A) 
    {
    pmergesort(N - half_length, res_arr + half_length, A + half_length);
    }
    #ifdef DEBUG
    printf("pmergesort arr2: ");
    print_arr(A + half_length, N - half_length);
    #endif
    #pragma omp taskwait
  }else{
    pmergesort(half_length, res_arr, A);
    pmergesort(N - half_length, res_arr + half_length, A + half_length);
  }
  smerge(half_length, A, N - half_length, A + half_length, res_arr);
  #ifdef DEBUG
  printf("merged_arr: ");
  print_arr(res_arr, N);
  #endif
}


void pmerge(int na, keytype* A, int nb, keytype* B, keytype* res_arr)
{
  keytype* long_arr = NULL;
  keytype* short_arr = NULL;
  int long_arr_length = 0;
  int short_arr_length = 0;
  if(na > nb){
    long_arr = A;
    short_arr = B;
    long_arr_length = na;
    short_arr_length = nb;
  }
  else{
    long_arr = B;
    short_arr = A;
    long_arr_length = nb;
    short_arr_length = na;
  }
  if(short_arr_length == 0){
    memcpy(res_arr, long_arr, long_arr_length * sizeof(keytype));
    return;
  }
  if(short_arr_length == 1){
    base_condition(*short_arr, long_arr_length, long_arr, res_arr);
    return;
  }
  int half_long = long_arr_length / 2; // index
  keytype v = *(long_arr + half_long); // value
  int half_short = binary_search(v, short_arr, short_arr_length); // index
  if(short_arr[half_short] < v){
    half_short++;
  }
  #ifdef DEBUG
  printf("value v=%d, half_short = %d\n", v, half_short);
  printf("short_arr: ");
  print_arr(short_arr, short_arr_length);
  printf("long_arr: ");
  print_arr(long_arr, long_arr_length);
  #endif
    // #pragma omp task shared(res_arr, A) if (long_arr_length > psize)
    // {
    pmerge(half_short, short_arr, half_long, long_arr, res_arr); // first half, A1 & B1
    // }
    #ifdef DEBUG
    printf("pmerge arr1: ");
    print_arr(res_arr, half_short + half_long);
    #endif
    // #pragma omp task shared(res_arr, A) if (long_arr_length > psize)
    // {
    pmerge(short_arr_length - half_short, short_arr + half_short, 
            long_arr_length - half_long, long_arr + half_long, 
            res_arr + half_short + half_long); // A2 & B2
    // }
    #ifdef DEBUG
    printf("pmerge arr2: ");
    print_arr(res_arr, na + nb - half_short - half_long);
    #endif
  // }
}

void smerge(int na, keytype* A, int nb, keytype* B, keytype* res_arr){
  int a = 0, b = 0, r = 0;
  while(a < na && b < nb){
    if(A[a] < B[b]){
      res_arr[r] = A[a];
      a++;
    }
    else{
      res_arr[r] = B[b];
      b++;
    }
    r++;
  }
  #ifdef DEBUG
  printf("a=%d, b=%d, r=%d\n", a, b, r);
  #endif
  if(a < na){
    memcpy(res_arr + r, A + a, sizeof(keytype) * (na -a));
  }
  if(b < nb){
    memcpy(res_arr + r, B + b, sizeof(keytype) * (nb - b));
  }
}

inline void base_condition(keytype num, int arr_length, keytype* arr, keytype* res_arr){
  int i = 0;
  *(res_arr + arr_length) = num;
  for(; i < arr_length; i++){
    if(*(arr + i) < num){
      *(res_arr + i) = *(arr + i);
    }else{
      *(res_arr + i) = num;
      break;
    }
  }
  memcpy(res_arr + i + 1, arr + i, sizeof(keytype) *(arr_length - i));
  #ifdef DEBUG
  printf("after base condition: ");
  print_arr(res_arr, arr_length + 1);
  #endif

}

int binary_search(keytype x, keytype* arr, int arr_length){
  int low = 0;
  int high = arr_length - 1;
  int mid = 0;
  while(low <= high){
    mid = (low + high) >> 1;
    if(x == arr[mid])
      return mid;
    else if (x > arr[mid])
      low = mid  + 1;
    else
      high = mid - 1;
  }
  return mid;
}

/* eof */
