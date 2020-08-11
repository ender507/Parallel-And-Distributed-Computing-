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
	int rest;		//现有产品数
	omp_init_lock(&lock);
	#pragma omp parallel shared(rest) 
	{
		rest = 0;		//现有产品数
		#pragma omp sections
		{
			//生产者 
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
			//消费者 
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
