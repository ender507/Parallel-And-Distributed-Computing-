#include<omp.h>
#include<stdio.h>
#include<cstdlib>
#include<ctime>
#include <sys/time.h>
#include<windows.h>
#define SIZE 1000000
float mNum[SIZE];
int row[SIZE],col[SIZE],vec[SIZE];
float ans[SIZE]={0};
//�������������ĵ�˲���ʱ��size������߳���valCount�������0Ԫ�ظ��� 
double calc(int size,int valCount){
	struct timeval start,end;
    int pos = 0;
    int count;
	//��ʱ��ʵ�����㲿�� 
	gettimeofday(&start,NULL);
	#pragma omp parallel for private(count,pos) num_threads(10)
    for(int i=1; i<size; i++){
    	//count��ʾ��i-1���ж��ٷ�0Ԫ�� 
    	count = col[i]-col[i-1];
		pos = col[i-1]-1;
		//�����е����з���Ԫ�ؽ��м��� 
		while(count--){
    		ans[row[pos]] += mNum[pos] * vec[row[pos]];
    		pos++;
		}
	}
	Sleep(5000);
	gettimeofday(&end,NULL);
	//���غ�ʱ����λΪ΢�� 
	return (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
} 

//������ת��Ϊѹ����ʽ��������ѹ���� 
void compressedRowFormat(FILE* mtx){
	int colpos = 1;
	col[0] = col[1] = 1;
	float num;
	int rown, coln;
	for(int i=0;1;i++){
		if(feof(mtx))break;
		fscanf(mtx,"%d%d%f",&rown,&coln,&num);
		//mtx��ʽ�µ��к��д�1��ʼ����������Ҫ��ȥ1 
		rown--;
		coln--;
		row[i] = rown;
		mNum[i] = num;
		if(coln == colpos-1)col[colpos]++;
		else{
			colpos++;
			col[colpos] = col[colpos-1]+1;
		}
	}
}

int main(){
	FILE* mtx = fopen("matrix.mtx","r");
	if(mtx==NULL){
		printf("Failed to open the file\n");
		return 0;
	}
	int tmp[3];
	fscanf(mtx,"%d%d%d",&tmp[0],&tmp[1],&tmp[2]);
	if(tmp[0]!=tmp[1]){
		printf("Not a square matrix\n");
		return 0;
	}
	//tmp�ֱ��¼�˾��������������������Ԫ�ظ�����Ϊ�˷���ͳ�ƣ��ṩ�ľ������������
	/*
	���ǵ�mtx��ʽ�´洢�ľ��󣬸���Ԫ�ذ��еĵ���˳�����У��洢ʱ���õ�����������£�
	mNum[i]:��i��Ԫ������ 
	row[i]:��i��Ԫ�������к�
	col[i]:��i��֮ǰ���з�0Ԫ�ظ���+1 
	*/ 
	compressedRowFormat(mtx); 
	//���վ����С������ɾ���Ԫ��ȡֵ[-2,2] 
	srand(time(NULL));
	for(int i=0;i<tmp[0];i++){
		vec[i]=rand()%3;			//��0��1��2��ȡֵ 
		if(rand()%2)vec[i]=-vec[i];	//1/2���ʱ�Ϊ���� 
	}
	double timeUsed = calc(tmp[0],tmp[2]);
	printf("�����СΪ%d*%d����0Ԫ�ظ���%d\n",tmp[0],tmp[0],tmp[2]);
	printf("������ʱ:%lf΢��\n",timeUsed-5000000);
};
