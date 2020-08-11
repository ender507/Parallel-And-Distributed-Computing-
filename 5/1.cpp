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
//计算矩阵和向量的点乘并计时。size：矩阵边长，valCount：矩阵非0元素个数 
double calc(int size,int valCount){
	struct timeval start,end;
    int pos = 0;
    int count;
	//计时与实际运算部分 
	gettimeofday(&start,NULL);
	#pragma omp parallel for private(count,pos) num_threads(10)
    for(int i=1; i<size; i++){
    	//count表示第i-1列有多少非0元素 
    	count = col[i]-col[i-1];
		pos = col[i-1]-1;
		//将本列的所有非零元素进行计算 
		while(count--){
    		ans[row[pos]] += mNum[pos] * vec[row[pos]];
    		pos++;
		}
	}
	Sleep(5000);
	gettimeofday(&end,NULL);
	//返回耗时，单位为微秒 
	return (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
} 

//将矩阵转换为压缩形式（采用列压缩） 
void compressedRowFormat(FILE* mtx){
	int colpos = 1;
	col[0] = col[1] = 1;
	float num;
	int rown, coln;
	for(int i=0;1;i++){
		if(feof(mtx))break;
		fscanf(mtx,"%d%d%f",&rown,&coln,&num);
		//mtx格式下的行和列从1开始计数，所以要减去1 
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
	//tmp分别记录了矩阵的行数、列数、非零元素个数，为了方便统计，提供的矩阵行列数相等
	/*
	考虑到mtx格式下存储的矩阵，各个元素按列的递增顺序排列，存储时所用的数组解释如下：
	mNum[i]:第i个元素数据 
	row[i]:第i个元素所在行号
	col[i]:第i列之前所有非0元素个数+1 
	*/ 
	compressedRowFormat(mtx); 
	//按照矩阵大小随机生成矩阵，元素取值[-2,2] 
	srand(time(NULL));
	for(int i=0;i<tmp[0];i++){
		vec[i]=rand()%3;			//在0、1、2间取值 
		if(rand()%2)vec[i]=-vec[i];	//1/2几率变为负数 
	}
	double timeUsed = calc(tmp[0],tmp[2]);
	printf("矩阵大小为%d*%d，非0元素个数%d\n",tmp[0],tmp[0],tmp[2]);
	printf("计算用时:%lf微秒\n",timeUsed-5000000);
};
