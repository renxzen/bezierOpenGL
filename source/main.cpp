#include "shader.h"
#include "window.h"

// puntos maximos: 21

int main(){
	Window ventana("curvasBezier");
	if(!ventana.start()) return -1;

	Shader programa("GLSL/code.vs","GLSL/code.fs");

	// Inicializar las curvas luego de la definicion del shader
	ventana.curvas = new Curves();

	// Loop Render
	while (!ventana.isClosed()){
		ventana.startLoop(0.4, 0.1, 0.2, 1.0);

		// Dibujar las curvas
		programa.usar();
		ventana.curvas->draw(programa);

		ventana.endLoop();
	}

	// glfw: Terminar
	glfwTerminate();
	return 0;
}