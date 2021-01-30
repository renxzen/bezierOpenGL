#include <bits/stdc++.h>
typedef long long ll;
using namespace std;

#define print(a) cout<<a<<"\n"
#define printArr(a) for(int i=0;i<sizeof(a)/sizeof(a[0]);i++) cout<<a[i]<<" "; cout<<"\n"

struct Factorials {
	long long array[20] = {1}; // factorials[0] = 1 
	Factorials() {
		for (int i = 1; i <= 20; i++) array[i] = i * array[i-1];
	}

	long long factorial(int n){
		return array[n];
	}

	int binomial(int n, int m) {
		return array[n]/(array[m]*array[n-m]);
	}
};

struct Punto {
	float x, y, z;
	Punto(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {}

	void printIt() {
		cout << x << " " << y << " " << z << endl;
	}

	Punto& setx(float a){
		x = a;
		return *this;
	}

	Punto& sety(float a){
		y = a;
		return *this;
	}

	Punto& operator=(Punto& punto){
		x = punto.x;
		y = punto.y;
		z = punto.z;
		return *this;
	}

};


int main() {
	Factorials fact;
	cout << fact.factorial(5) << endl;
	cout << fact.binomial(7,2) << endl;

	vector<Punto> puntosControl;
	for(int i=0; i<5; i++){
		puntosControl.push_back(Punto());
		puntosControl[i].setx(i+i).sety(i*i).printIt();
	}

}