#include "shader.h"
#include "window.h"

// puntos maximos: 21

int main(){
	Window ventana("curvasBezier");
	if(!ventana.start()) return -1;

	Shader programa("GLSL/code.vs","GLSL/code.fs");

	Curves* curvas = new Curves();
	ventana.curvas = curvas;

	
	// render loop
	while (!ventana.isClosed()){
		ventana.startLoop(0.4, 0.1, 0.2, 1.0);

		// draw our first triangle
		programa.usar();

		curvas->draw(programa);
 
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		ventana.endLoop();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}