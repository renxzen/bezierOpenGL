#include <bits/stdc++.h>
typedef long long ll;
using namespace std;

#define print(a) cout<<a<<"\n"
#define printArr(a) for(int i=0;i<sizeof(a)/sizeof(a[0]);i++) cout<<a[i]<<" "; cout<<"\n"

struct Factorials {
	ll array[20] = {1}; // factorials[0] = 1 
	ll bin[20][20];

	Factorials() {
		for (int i = 1; i <= 20; i++) array[i] = i * array[i-1];

		bin[0][0] = 1;
		for (int i = 1; i <= 20; i++) {
			bin[i][0] = 1;
			bin[0][i] = 0;
		}

		for (int i = 1; i <= 20; i++) {
			for (int j = 1; j <= 20; j++) {
				if (i < j) bin[i][j] = -1;
				else bin[i][j] = array[i]/(array[j]*array[i-j]);
			}
		}
	}

	long long factorial(int n){
		return array[n];
	}

	long long binomial(int n, int m) {
		return bin[n][m];
	}

};

int main() {
	cout << "okay\n";

	Factorials fact;
	cout << fact.factorial(5) << endl;

	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			cout << fact.binomial(i,j) << "\t";
		}
		cout << endl;
	}

}