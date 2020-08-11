/*		test.cpp	*/
#include<omp.h>
#include<stdio.h>

int main(){
	printf("%d",omp_get_num_procs());
}


