#include <omp.h>
#include <iostream>
#include <sys/time.h>
using namespace std;

int vec1[1000000],vec2[1000000],result[1000000];

int main(){
	//���ݳ�ʼ�� 
	for(int i=0; i<1000000; i++){
		vec1[i]=1;
		vec2[i]=2;
	}
	struct timeval start,end; 
	//˳��ִ��
	gettimeofday(&start,NULL);
	for(int i=0; i<1000000; i++){
		result[i] = vec1[i]+vec2[i];
	}
	gettimeofday(&end,NULL);
	double extime = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
	std::cout << "˳��ִ�мӺ�������������ʱΪ" << extime << "΢��" << std::endl;
	//����ִ�� 
	gettimeofday(&start,NULL);
	#pragma omp parallel for
	for(int i=0; i<1000000; i++){
		result[i] = vec1[i]+vec2[i];
	}
	gettimeofday(&end,NULL);
	extime = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
	std::cout << "���߳�ִ�мӺ�������������ʱΪ" << extime << "΢��" << std::endl;
    return 0;
}
