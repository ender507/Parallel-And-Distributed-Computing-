#include<stdio.h>
#include<pthread.h>
#include<sys/time.h>
#define SIZE 1000

float vec[1000] = {0};
float matrix[1000][1000] = {0};
float answer[1000] = {0};

//��¼��Ҫ���ʵľ����еı߽� 
struct border{
	int left;
	int right;
};

void *calc(void *ptr){
	struct border *bd = (struct border*) ptr;
	for(int i=bd->left; i<bd->right; i++){
		for(int j=0; j<SIZE; j++){
			answer[i] += (vec[j]*matrix[i][j]);
		}
	}
}

int main(){
	struct timeval start,end;
	gettimeofday(&start,NULL); 			//��¼��ʼʱ�� 
	int num = 1000 / 12;
	int l = 0;
	int ret_tid[12];
	pthread_t tid[12];
	struct border bd[12];
	for(int i=0; i<12; i++){
		//�趨��ǰ�̷߳��ʵľ����з�Χ�ı߽� 
		bd[i].left = l;
		if(i<=3)bd[i].right = bd[i].left+83;
		else bd[i].right = bd[i].left+82;
		l = bd[i].right + 1; 
		ret_tid[i] = pthread_create(&tid[i],NULL,calc,(void*)(&bd[i]));
		if(ret_tid[i])printf("�����߳�ʧ��\n");
	}
	for(int i=0; i<12; i++){
		pthread_join(tid[i], NULL);
	}
	gettimeofday(&end,NULL);			//��¼����ʱ�� 
	printf("��ʱ%lld΢��\n",(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
}

