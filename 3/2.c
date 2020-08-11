#include<stdio.h>
#include<pthread.h>

int Num;
static int Num2;

void *assignNum(){
	Num = 1;
	Num2 = 2;
	return NULL;
}

int main(){
	pthread_t ptds[2];
	for(int i=0; i<2; i++){
		pthread_create(&ptds[i], NULL, assignNum, NULL);
	}
	for(int i=0; i<2; i++)pthread_join(ptds[i],NULL);
	return 0;
}

