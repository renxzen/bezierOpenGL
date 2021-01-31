#pragma once

#include "shader.h"
#include <vector>

using namespace std;

struct Factorials {
	long long array[20] = {1}; // factorials[0] = 1 
	Factorials() {
		for (int i = 1; i <= 20; i++) array[i] = i * array[i-1];
	}

	long long factorial(int n){
		return array[n];
	}

	long long binomial(int n, int m) {
		return array[n]/(array[m]*array[n-m]);
	}

};

struct Point {
	float x, y, z;
	Point(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {}
	~Point() {}

	Point& setx(float a){
		x = a;
		return *this;
	}

	Point& sety(float a){
		y = a;
		return *this;
	}

	Point& operator=(Point& punto){
		x = punto.x;
		y = punto.y;
		z = punto.z;
		return *this;
	}

};

struct Curve {
	// Data
	vector<Point> points;
	vector<float> coordinates;
	unsigned int VBO, VAO;
	
	vector<float> bezier;
	unsigned int bVBO, bVAO;

	// Constructor & Destructor
	Curve() {}
	~Curve() {
		glDeleteVertexArrays(1, &VAO);
    	glDeleteBuffers(1, &VBO);

		glDeleteVertexArrays(1, &bVAO);
    	glDeleteBuffers(1, &bVBO);
	}

	// Functions

	void addPoint(float x, float y, float z = 0) {
		coordinates.push_back(x);
		coordinates.push_back(y);
		coordinates.push_back(z);
		points.push_back(Point(x,y,z));
	}
	
	void updateBezier(Factorials &fact){
		if (points.size() >= 2){
			bezier.clear();
			for (float t= 0.0; t <= 1.0; t+=0.005){
				float x = 0, y = 0;
				int n = points.size() - 1;
				for (int i = 0; i< n+1; i++){
					x += fact.binomial(n,i) * pow((1-t),n-i) * pow(t,i) * points[i].x;
					y += fact.binomial(n,i) * pow((1-t),n-i) * pow(t,i) * points[i].y;
				}
				bezier.push_back(x);
				bezier.push_back(y);
				bezier.push_back(0);
			}
		}
	}
	

	void draw(Shader &shader) {	
		// Dibujar los puntos
		if (points.size() > 0) {
			// Establecer el arreglo y el bufer de vertices
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			// Enlazar el Objeto de arreglo de vertices primero y luego configurar el bufer de vertice y los atributos
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(float), coordinates.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); 

			// Desenlazar el buffer y el puntero al array de vertice
			glBindBuffer(GL_ARRAY_BUFFER, 0); 
			glBindVertexArray(0); 

			// Enlazar el arreglo
			glBindVertexArray(VAO);

			// Dibujar los puntos
			shader.setVec3("colorize", glm::vec3(1.0, 0.5, 0.2));
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, coordinates.size()/3);
			
			// Dibujar las lineas
			if (points.size() >= 2) {
				shader.setVec3("colorize", glm::vec3(1.0, 1.0, 1.0));
				glLineWidth(1.35);
				glDrawArrays(GL_LINE_STRIP, 0, coordinates.size()/3);
			}

			// Desenlazar a default
			glBindVertexArray(0);
		}
		
		
		// Dibujar las Bezier
		if (points.size() > 2) {
			// Establecer el arreglo y el bufer de vertices
			glGenVertexArrays(1, &bVAO);
			glGenBuffers(1, &bVBO);

			// Enlazar el Objeto de arreglo de vertices primero y luego configurar el bufer de vertice y los atributos
			glBindVertexArray(bVAO);
			glBindBuffer(GL_ARRAY_BUFFER, bVBO);
			glBufferData(GL_ARRAY_BUFFER, bezier.size() * sizeof(float), bezier.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// Desenlazar el buffer y el puntero al array de vertice
			glBindBuffer(GL_ARRAY_BUFFER, 0); 
			glBindVertexArray(0); 

			// Enlazar el arreglo
			glBindVertexArray(bVAO);

			// Dibujar la linea de bezier
			shader.setVec3("colorize", glm::vec3(0.0, 0.0, 1.0));
			glLineWidth(1.35);
			glDrawArrays(GL_LINE_STRIP, 0, bezier.size()/3);
			
			// Desenlazar a default
			glBindVertexArray(0);
		}
		
	
	}
};
