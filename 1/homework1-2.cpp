#include <omp.h>
#include <iostream>
#include <sys/time.h>
using namespace std;

int vec1[1000000],vec2[1000000],result[1000000];

int main(){
	//数据初始化 
	for(int i=0; i<1000000; i++){
		vec1[i]=1;
		vec2[i]=2;
	}
	struct timeval start,end; 
	//顺序执行
	gettimeofday(&start,NULL);
	for(int i=0; i<1000000; i++){
		result[i] = vec1[i]+vec2[i];
	}
	gettimeofday(&end,NULL);
	double extime = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
	std::cout << "顺序执行加和两个向量，用时为" << extime << "微秒" << std::endl;
	//并行执行 
	gettimeofday(&start,NULL);
	#pragma omp parallel for
	for(int i=0; i<1000000; i++){
		result[i] = vec1[i]+vec2[i];
	}
	gettimeofday(&end,NULL);
	extime = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
	std::cout << "多线程执行加和两个向量，用时为" << extime << "微秒" << std::endl;
    return 0;
}
