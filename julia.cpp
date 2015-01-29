#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cmath>
using namespace std;

int main(){
	int max=256;
	int X=200, Y=100;
	double cr,ci;
	map<vector<double>,long> colors;
	cin >> cr >> ci;
	for (int j=0;j<Y;j++){
		for (int i=0; i<X;i++){
			double real=1.7*(i-0.5*X)/(0.5*X);
			double imag=1.7*(j-0.5*Y)/(0.5*Y);
			vector<double> coord;
			coord.push_back(i);
			coord.push_back(j);
			double magsq;
			double oldReal;
			int iter=max;
			for (int k=1;k<=max;k++){
				oldReal=real;
				real=(oldReal*oldReal)-(imag*imag)+cr;
				imag=2*oldReal*imag+ci;
				magsq=real*real+imag*imag;
				if (magsq>=4){
					iter=k;
					break;
				}
			}
			colors[coord]=iter;
			if (iter==max){
				cout << '*';
			}else{
				cout<< ' ';
			}
		}
		cout << '\n';
	}
	return 0;
}