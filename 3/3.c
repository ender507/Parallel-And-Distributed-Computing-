#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *assignNum(){
	printf("assignNum\n");
	return NULL;
}

int main(){
	pthread_t ptds[1];
	for(int i=0; i<2; i++){
		pthread_create(&ptds[i],NULL,assignNum,NULL);
	}
	for(int i=0; i<2; i++)pthread_join(ptds[i],NULL);
	return 0;
}

