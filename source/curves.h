#pragma once

#include "structs.h"

struct Curves {
	// Data
	vector<Curve> curvesVec;
	Factorials fact;

	// Constructor & Destructor
	Curves() {
		curvesVec.push_back(Curve());
	}
	~Curves() {}

	// Functions
	void addPointToCurrentCurve(float x, float y){
		curvesVec[curvesVec.size()-1].addPoint(x, y);
		cout << "point added in " << x << "," << y << endl;
	}

	void addCurve(){
		curvesVec.push_back(Curve());
	}

	void draw(Shader &shader){
		for (int i=0; i < curvesVec.size(); i++) {
			curvesVec[i].updateBezier(fact);
			curvesVec[i].draw(shader);
		}
	}

	void deallocate() {
		for (int i=0; i < curvesVec.size(); i++) curvesVec[i].deallocate();
	}

};