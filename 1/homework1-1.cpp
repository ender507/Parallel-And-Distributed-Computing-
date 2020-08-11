#include<immintrin.h>
#include<iostream>
#include <sys/time.h>

//10^6��������3.2*10^7λ��ÿ��__m256i���͵�������256λ������Ҫ125000��__m256i���10^6�������� 
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
	std::cout << "ֱ��˳��Ӻ�������������ʱΪ" << extime << "΢��" << std::endl;
}

void par_add(__m256i *par_vec1,__m256i *par_vec2,__m256i *par_result){
	struct timeval start,end;
    gettimeofday(&start,NULL);
	for(int i=0; i<125000; i++){
		par_result[i] = _mm256_add_epi32(par_vec1[i],par_vec2[i]);
	}
	gettimeofday(&end,NULL);
	double extime = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
	std::cout << "ʹ��AVX�Ĳ���ָ��Ӻ�������������ʱΪ" << extime << "΢��" << std::endl;
}

int main(){	
	//���ݳ�ʼ�� 
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
