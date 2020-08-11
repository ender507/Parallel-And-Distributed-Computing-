#include<immintrin.h>
#include<iostream>
#include <sys/time.h>

//10^6个整数有3.2*10^7位，每个__m256i类型的数据有256位，故需要125000个__m256i存放10^6个整数。 
__m256i par_vec1[125000],par_vec2[125000],par_result[125000];
int seq_vec1[1000000],seq_vec2[1000000],seq_result[1000000];

void seq_add(int *seq_vec1,int *seq_vec2,int *seq_result){
	struct timeval start,end;
    gettimeofday(&start,NULL);
	for(int i=0; i<1000000; i++){
		seq_result[i] = seq_vec1[i]+seq_vec2[i];
	}
	gettimeofday(&end,NULL);
	double extime = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
	std::cout << "直接顺序加和两个向量，用时为" << extime << "微秒" << std::endl;
}

void par_add(__m256i *par_vec1,__m256i *par_vec2,__m256i *par_result){
	struct timeval start,end;
    gettimeofday(&start,NULL);
	for(int i=0; i<125000; i++){
		par_result[i] = _mm256_add_epi32(par_vec1[i],par_vec2[i]);
	}
	gettimeofday(&end,NULL);
	double extime = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
	std::cout << "使用AVX的并行指令加和两个向量，用时为" << extime << "微秒" << std::endl;
}

int main(){	
	//数据初始化 
	for(int i=0; i<125000; i++){
		par_vec1[i] = _mm256_set1_epi32(1);
		par_vec2[i] = _mm256_set1_epi32(2);
	}
	for(int i=0;i<1000000;i++){
		seq_vec1[i]=1;
		seq_vec2[i]=2;
	}
	
	seq_add(seq_vec1,seq_vec2,seq_result);
	par_add(par_vec1,par_vec2,par_result);
	
	return 0;
}
