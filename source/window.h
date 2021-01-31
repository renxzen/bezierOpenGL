#pragma once

#include "shader.h"
#include "camera.h"
#include "stb_image.h"
#include "curves.h"

class Window{
private:
	// Nada
public:
	// Data
	GLFWwindow* window = nullptr;
	Camera* camara = nullptr;
	unsigned int width, height;
	string titulo;
	float wR = 0.0, wG = 0.0, wB = 0.0, wA = 1.0;
	float tiempo = 0.0, sinTiempo = 0.0, cosTiempo = 0.0;
	
	// Transformaciones de camara
	glm::mat4 proyeccion, vista;
	
	// Curvas de Bezier
	Curves* curvas = nullptr;
	bool limpiarCurvas = false;

	//------------------------------------------------------------------------
	// Constructor
	Window(string tittle, unsigned int width = 800, unsigned int height = 600) : titulo(tittle), width(width), height(height) {
		camara = new Camera(glm::vec3(0.0, 0.0, 3.0));
		//curvas = new Curves();

		// Cambiar los valores de camara con las dimensiones de la ventana
		camara->lastX = width / 2.0;
		camara->lastY = height / 2.0;
	};

	// Destructor
	~Window() {
		camara = nullptr; delete camara;
		curvas = nullptr; delete curvas;
	};

	//------------------------------------------------------------------------
	// Iniciar ventana
	bool start() {
		//Inicializar glfw
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//Creando la ventana
		window = glfwCreateWindow(width, height, titulo.c_str(), NULL, NULL);
		if (window == NULL) {
			cout << "Problemas al crear la ventana\n";
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);

		// Dirigir el puntero de Ventana en las funciones  glfw al de esta misma clase
		glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

		// Definicion de lambdas para utilizar las funciones de clase en las funciones callback
		auto resizingWindow = [](GLFWwindow* window, int ancho, int alto) {
			static_cast<Window*>(glfwGetWindowUserPointer(window))->framebufferSizeCallback(window, ancho, alto);
		};
		auto mouseMovement = [](GLFWwindow* window, double xpos, double ypos) {
			static_cast<Window*>(glfwGetWindowUserPointer(window))->mouseMovementCallback(window, xpos, ypos);
		};
		auto mouseScroll = [](GLFWwindow* window, double xoffset, double yoffset) {
			static_cast<Window*>(glfwGetWindowUserPointer(window))->mouseScrollCallback(window, xoffset, yoffset);
		};

		auto mouseButton = [](GLFWwindow* window, int button, int action, int mods) {
			static_cast<Window*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
		};


		// GLFW Callbacks
		glfwSetFramebufferSizeCallback(window, resizingWindow); // resizing window

		glfwSetCursorPosCallback(window, mouseMovement); // mouseMovement
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // GLFW_CURSOR_DISABLED Para que GLFW capture el mouse

		glfwSetScrollCallback(window, mouseScroll); // mouseScroll

		glfwSetMouseButtonCallback(window, mouseButton); //mouseButton
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);


		//Cargar Glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			cout << "Problemas al cargar GLAD\n";
			return false;
		}

		// Decirle a stb_image.h que voltee la textura cargada en el eje y (antes de que cargue el modelo)
		stbi_set_flip_vertically_on_load(true);

		// Activar la profundidad
		glEnable(GL_DEPTH_TEST);

		// Poligonos wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Todo fue cargado correctamente
		return true;
	}

	// Revisar si la ventana se ha cerrado
	bool isClosed() {
		return glfwWindowShouldClose(window);
	}

	void setColor(float r = -1, float g = -1, float b = -1, float a = -1) {
		if (r >= 0) wR = r;
		if (g >= 0) wG = g;
		if (b >= 0) wB = b;
		if (a >= 0) wA = a;
	}

	// Codigo generico
	void startLoop(float r = -1, float g = -1, float b = -1, float a = -1) {
		// Logica del tiempo
		tiempo = (float)glfwGetTime();
		sinTiempo = glm::sin(tiempo);
		cosTiempo = glm::cos(tiempo);
		
		camara->tiempoDelta = tiempo - camara->ultimoFrame;
		camara->ultimoFrame = tiempo;

		// Transformaciones de camara
		proyeccion = glm::perspective(glm::radians(camara->zoom), (float)width / (float)height, 0.1f, 100.0f);
		vista = camara->getMatrizVista();

		// Procesa input
		procesarEntrada(); 

	 	// Render - Color de fondo // Limpiar y colorear el fondo de la ventana
		setColor(r, g, b, a);
		glClearColor(wR,wG,wB,wA);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	void endLoop() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Procesar todos los inputs
	void procesarEntrada() {
		// Escape para cerrar
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

		// Movimiento de Camara
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camara->procesarTeclado(ADELANTE);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camara->procesarTeclado(ATRAS);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camara->procesarTeclado(IZQUIERDA);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camara->procesarTeclado(DERECHA);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camara->procesarTeclado(ARRIBA);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camara->procesarTeclado(ABAJO);

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !limpiarCurvas) {
			limpiarCurvas ^= 1;
			curvas->curvesVec.clear();
			curvas->curvesVec.push_back(Curve());
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && limpiarCurvas) limpiarCurvas ^= 1;

	}

	//------------------------------------------------------------------------
	// Definiendo las funciones glfw callback
	
	// glfw: cuando el tamaño de la ventana es cambiado
	void framebufferSizeCallback(GLFWwindow* window, int ancho, int alto) {
		glViewport(0, 0, ancho, alto);
	}

	// glfw: cuando el mouse se mueve
	void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos) {
		if (camara->primerMouse) {
			camara->lastX = xpos;
			camara->lastY = ypos;
			camara->primerMouse = false;
		}
		float xoffset = xpos - camara->lastX;
		float yoffset = camara->lastY - ypos; // reversed since y-coordinates go from bottom to top
		camara->lastX = xpos;
		camara->lastY = ypos;
		camara->procesarMovMouse(xoffset, yoffset);
	}

	// glfw: cuando los botones del mouse son presionados
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		// Generar puntos
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			curvas->addPointToCurrentCurve((2*camara->lastX/width)-1, 1-(2*camara->lastY/height));

		// Empezar nueva curva
    	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			curvas->addCurve();
	}

	// glfw: cuando la rueda del mouse se mueve
	void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		camara->procesarRuedaMouse(yoffset);
	}
};