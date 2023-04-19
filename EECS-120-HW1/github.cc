#include "sort.hh"

#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

using namespace std;

inline void  merge  (keytype *datal , int sizel , keytype *datar , int sizer , keytype *data2){
    int i1 = 0, i2 = 0, i3 = 0;
       
    while (i1 < sizel && i2 < sizer) {
        if ( datal[i1] < datar[i2]) 
            data2[i3++] = datal[i1++];
        else 
            data2[i3++] = datar[i2++];
    }
	while(i1<sizel)
		data2[i3++] = datal[i1++];
	while(i2<sizel)
		data2[i3++] = datar[i2++];
}

void msort(keytype *data, int size, keytype *data2){
	if (size>1){
		int siz = size>>1;
		int siz1 = (size+1)>>1;
		#pragma omp task if (size > 1<<20)
		{
			msort(data2,siz,data);
		}
		#pragma omp task if (size > 1<<20)
		{
			msort(data2+siz, siz1, data+siz);
		}

		#pragma omp taskwait
		merge(data2,siz,data2+siz, siz1,data);
	}
}

void mergesort(keytype *data, int size){
	keytype* data2=(keytype*)malloc(size*sizeof(keytype));
	if (data2==0){
		cout<<"OUT OF MEMORY\n";
		exit(0);
	}
	#pragma omp parallel   
	{
		//omp_set_nested(1);
		// copy all the data to data2
		#pragma omp for 
		for (int i=0; i<size; i++){
			data2[i] = data[i];
		}

		#pragma omp master
		{
			msort(data, size, data2);	
		}
	}
	free(data2);
}

