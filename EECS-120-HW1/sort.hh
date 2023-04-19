/**
 *  \file sort.hh
 *
 *  \brief Interface to sorting arrays of keys ('keytype' values).
 */

#if !defined (INC_SORT_HH)
#define INC_SORT_HH /*!< sort.hh already included */

/** 'keytype' is the primitive type for sorting keys */
typedef unsigned long keytype;

/**
 *  Sorts an input array containing N keys, A[0:N-1]. The sorted
 *  output overwrites the input array.
 */
void quickSort (int N, keytype* A);

/**
 *  Sorts an input array containing N keys, A[0:N-1]. The sorted
 *  output overwrites the input array. This is the routine YOU will
 *  implement; see 'mergesort-omp.cc'.
 */
void mySort (int N, keytype* A);

/** Returns a new uninitialized array of length N */
keytype* newKeys (int N);

/** Returns a new copy of A[0:N-1] */
keytype* newCopy (int N, const keytype* A);

/**
 *  Checks whether A[0:N-1] is in fact sorted, and if not, aborts the
 *  program.
 */
void assertIsSorted (int N, const keytype* A);

/**
 *  Checks whether A[0:N-1] == B[0:N-1]. If not, aborts the program.
 */
void assertIsEqual (int N, const keytype* A, const keytype* B);


inline void simple_sort(int N, keytype* A, keytype* res_arr);

void pmergesort(int N, keytype* A, keytype* res_arr);

void pmerge(int na, keytype* A, int nb, keytype* B, keytype* res_arr);

void base_condition(keytype num, int arr_length, keytype* arr, keytype* res_arr);

int binary_search(keytype x, keytype* arr, int arr_length);

void print_arr(keytype* A, int arr_length);

inline void  merge  (keytype *datal , int sizel , keytype *datar , int sizer , keytype *data2);
void msort(keytype *data, int size, keytype *data2);
void mergesort(keytype *data, int size);

#endif

/* eof */
