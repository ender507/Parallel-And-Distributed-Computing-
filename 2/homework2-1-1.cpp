#include<vector>
#define LEN 300
using namespace std;
int main(){
	vector<int> inner1(LEN,1),inner2(LEN,0);
	vector<vector<int> >vec1,vec2,result;
	for(int i=0; i<LEN; i++){
		vec1.push_back(inner1);
		vec2.push_back(inner1);
		result.push_back(inner2);
	}
	for(int i=0; i<300; i++){
		for(int j=0; j<300; j++){
			result[i][j] = 0;
			for(int k=0; k<300; k++){
				result[i][j] += (vec1[i][k] * vec2[k][j]);
			}
		}
	}
}
