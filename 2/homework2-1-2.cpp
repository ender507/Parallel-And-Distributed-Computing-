#define LEN 300
#include<vector>
using namespace std;
void mult(vector<vector<int> >&vec1,vector<vector<int> >&vec2,vector<vector<int> >&result,int l1,int r1,int u1,int d1,int l2,int r2,int u2,int d2){
	//��ֺ��vec1ֻ��һ�У�vec2ֻ��һ�У���ֱ�Ӽ��� 
	if( u1==d1-1 && l2==r2-1){
		for(int i=l1; i!=r1; i++)result[u1][l2] += (vec1[u1][i]*vec2[i][l2]);
		return;
	}
	//��ֺ��vec1ֻ��һ�У�vec2�ж��У���ֻ��vec2���в�� 
	if(u1==d1-1){
		for(int i=l2;i!=r2;i++)mult(vec1,vec2,result,l1,r1,u1,d1,i,i+1,u2,d2);
		return;
	}
	//��ֺ��vec1�ж��У�vec2ֻ��һ�У���ֻ��vec1���в��
	if(l2==r2-1){
		for(int i=u1;i!=d1;i++)mult(vec1,vec2,result,l1,r1,i,i+1,l2,r2,u2,d2);
		return;
	}
	//���򣬽��ж԰���,�������Ϊ2x2���Ӿ��� 
	//����(0,0)λ�õ��Ӿ��� 
	mult(vec1,vec2,result,l1,(l1+r1)/2,u1,(u1+d1)/2,l2,(l2+r2)/2,u2,(u2+d2)/2);
	mult(vec1,vec2,result,(l1+r1)/2,r1,u1,(u1+d1)/2,l2,(l2+r2)/2,(u2+d2)/2,d2);
	//����(0,1)λ�õ��Ӿ���
	mult(vec1,vec2,result,l1,(l1+r1)/2,u1,(u1+d1)/2,(l2+r2)/2,r2,u2,(u2+d2)/2);
	mult(vec1,vec2,result,(l1+r1)/2,r1,u1,(u1+d1)/2,(l2+r2)/2,r2,(u2+d2)/2,d2);
	//����(1,0)λ�õ��Ӿ���
	mult(vec1,vec2,result,l1,(l1+r1)/2,(u1+d1)/2,d1,l2,(l2+r2)/2,u2,(u2+d2)/2);
	mult(vec1,vec2,result,(l1+r1)/2,r1,(u1+d1)/2,d1,l2,(l2+r2)/2,(u2+d2)/2,d2);
	//����(1,1)λ�õ��Ӿ���
	mult(vec1,vec2,result,l1,(l1+r1)/2,(u1+d1)/2,d1,(l2+r2)/2,r2,u2,(u2+d2)/2);
	mult(vec1,vec2,result,(l1+r1)/2,r1,(u1+d1)/2,d1,(l2+r2)/2,r2,(u2+d2)/2,d2);
}

int main(){
	vector<int> inner1(LEN,1),inner2(LEN,0);
	vector<vector<int> >vec1,vec2,result;
	for(int i=0; i<LEN; i++){
		vec1.push_back(inner1);
		vec2.push_back(inner1);
		result.push_back(inner2);
	}
	mult(vec1,vec2,result,0,LEN,0,LEN,0,LEN,0,LEN);
} 
