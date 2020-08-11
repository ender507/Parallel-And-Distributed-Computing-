#define LEN 512	//矩阵大小为了适应strassen算法，扩大为512（为2的整数次幂） 
#include<vector>
#include<iostream>
using namespace std;
int times;
void strassen(vector<vector<int> >&vec1,vector<vector<int> >&vec2,vector<vector<int> >&result,int len){
	times++;
	if(len == 1){
		result[0][0] = vec1[0][0] * vec2[0][0];
		return;
	}
	vector<int> inner(len/2,0);
	vector<vector<int> > a11(len/2,inner),a12(len/2,inner),a21(len/2,inner),a22(len/2,inner),b11(len/2,inner),b12(len/2,inner),b21(len/2,inner),b22(len/2,inner);
	for(int i=0; i<len; i++){
		for(int j=0; j<len; j++){
			if(i<len/2 && j<len/2){
				a11[i][j] = vec1[i][j];
				b11[i][j] = vec2[i][j];
			}
			else if(i<len/2 && j>=len/2){
				a12[i][j-len/2] = vec1[i][j];
				b12[i][j-len/2] = vec2[i][j]; 
			}
			else if(i>=len/2 && j<len/2){
				a21[i-len/2][j] = vec1[i][j];
				b21[i-len/2][j] = vec2[i][j];
			}
			else{
				a22[i-len/2][j-len/2] = vec1[i][j];
				b22[i-len/2][j-len/2] = vec2[i][j];
			}
		}
	}
	vector<vector<int> >s1(len/2,inner),s2(len/2,inner),s3(len/2,inner),s4(len/2,inner),s5(len/2,inner),s6(len/2,inner),s7(len/2,inner),s8(len/2,inner),s9(len/2,inner),s10(len/2,inner);
	for(int i=0; i<len/2; i++)for(int j=0; j<len/2; j++){
		s1[i][j] = b12[i][j] - b22[i][j];
		s2[i][j] = a11[i][j] + a12[i][j];
		s3[i][j] = a21[i][j] + a22[i][j];
		s4[i][j] = b21[i][j] - b11[i][j];
		s5[i][j] = a11[i][j] + a22[i][j];
		s6[i][j] = b11[i][j] + b22[i][j];
		s7[i][j] = a12[i][j] - a22[i][j];
		s8[i][j] = b21[i][j] + b22[i][j];
		s9[i][j] = a11[i][j] - a21[i][j];
		s10[i][j] = b11[i][j] + b12[i][j];
	}
	vector<vector<int> >p1(len/2,inner),p2(len/2,inner),p3(len/2,inner),p4(len/2,inner),p5(len/2,inner),p6(len/2,inner),p7(len/2,inner);
	strassen(a11,s1,p1,len/2);
	strassen(s2,b22,p2,len/2);
	strassen(s3,b11,p3,len/2);
	strassen(a22,s4,p4,len/2);
	strassen(s5,s6,p5,len/2);
	strassen(s7,s8,p6,len/2);
	strassen(s9,s10,p7,len/2);
	for(int i=0; i<len; i++)for(int j=0; j<len; j++){
		if(i<len/2 && j<len/2) result[i][j] = p5[i][j] + p4[i][j] - p2[i][j] + p6[i][j];
		else if(i<len/2 && j>=len/2) result[i][j] = p1[i][j-len/2] + p2[i][j-len/2];
		else if(i>=len/2 && j<len/2) result[i][j] = p3[i-len/2][j] + p4[i-len/2][j];
		else result[i][j] = p5[i-len/2][j-len/2] +p1[i-len/2][j-len/2] - p3[i-len/2][j-len/2] - p7[i-len/2][j-len/2];
	}
}
int main(){
	times=0;
	vector<int> inner1(LEN,1),inner2(LEN,0);
	vector<vector<int> > vec1,vec2,result;
	for(int i=0; i<LEN; i++){
		vec1.push_back(inner1);
		vec2.push_back(inner1);
		result.push_back(inner2);
	}
	strassen(vec1,vec2,result,LEN);
	cout<<times;
}
