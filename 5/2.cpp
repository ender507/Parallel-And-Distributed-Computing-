#include<omp.h>
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>

static omp_lock_t lock;

void produce(){
	Sleep(100*(rand()%10));
}
void consume(){
	Sleep(100*(rand()%10));
}

int main(){
	int rest;		//���в�Ʒ��
	omp_init_lock(&lock);
	#pragma omp parallel shared(rest) 
	{
		rest = 0;		//���в�Ʒ��
		#pragma omp sections
		{
			//������ 
			#pragma omp section
			{
				while(1){
					produce();
					while(rest==10);
					omp_set_lock(&lock);
					rest++;
					printf("rest+1,rest=%d\n\n",rest);
					omp_unset_lock(&lock);
				}
			}
			//������ 
			#pragma omp section
			{
				while(1){
					while(rest==0);
					omp_set_lock(&lock);
					rest--;
					printf("rest-1,rest=%d\n\n",rest);
					omp_unset_lock(&lock);
					consume();
				}
			}
		}
	} 
}
