#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Shader {
private:
	void verificarErrores(GLuint identificador, string tipo) {
		GLint ok;
		GLchar log[1024];
		if (tipo == "Programa") {
			glGetProgramiv(idPrograma, GL_LINK_STATUS, &ok);
			if (!ok) {
				glGetProgramInfoLog(idPrograma, 1024, NULL, log);
				cout << "Error de enlace con el programa: " << log << "\n";
			}
		}
		else {
			glGetShaderiv(identificador, GL_COMPILE_STATUS, &ok);
			if (!ok) {
				glGetShaderInfoLog(identificador, 1024, nullptr, log);
				cout << "Error de compilacion con el Shader de " << tipo << ": " << log << "\n";
			}
		}
	}
public:
	// ID
	GLuint idPrograma;
	//------------------------------------------------------------------------
	// Constructor
	Shader(string rutaShaderVertice, string rutaShaderFragmento, string rutaShaderGeometria = "") {
		//Variables para leer los archivos de c�digo
		string strCodigoShaderVertice;
		string strCodigoShaderFragmento;
		string strCodigoShaderGeometria;
		ifstream pArchivoShaderVertice;
		ifstream pArchivoShaderFragmento;
		ifstream pArchivoShaderGeometria;
		//Mostramos excepciones en caso haya
		pArchivoShaderVertice.exceptions(ifstream::failbit | ifstream::badbit);
		pArchivoShaderFragmento.exceptions(ifstream::failbit | ifstream::badbit);
		pArchivoShaderGeometria.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			//Abriendo los archivos de c�digo de los shader
			pArchivoShaderVertice.open(rutaShaderVertice);
			pArchivoShaderFragmento.open(rutaShaderFragmento);
			//Leyendo la informaci�n de los archivos
			stringstream lectorShaderVertice, lectorShaderFragmento;
			lectorShaderVertice << pArchivoShaderVertice.rdbuf();
			lectorShaderFragmento << pArchivoShaderFragmento.rdbuf();
			//Cerrando los archivos
			pArchivoShaderVertice.close();
			pArchivoShaderFragmento.close();
			//Pasando la informaci�n leida a string
			strCodigoShaderVertice = lectorShaderVertice.str();
			strCodigoShaderFragmento = lectorShaderFragmento.str();
			//Shader de Geometria
			if (rutaShaderGeometria != "") {
				pArchivoShaderGeometria.open(rutaShaderGeometria);
				stringstream lectorShaderGeometria;
				lectorShaderGeometria << pArchivoShaderGeometria.rdbuf();
				pArchivoShaderGeometria.close();
				strCodigoShaderGeometria = lectorShaderGeometria.str();
			}
		}
		catch (ifstream::failure) {
			cout << "ERROR: Los archivos no pudieron ser leidos correctamente.\n";
		}
		const char* codigoShaderVertice = strCodigoShaderVertice.c_str();
		const char* codigoShaderFragmento = strCodigoShaderFragmento.c_str();
		//Asociando y compilando los c�digos de los shader
		GLuint idShaderVertice, idShaderFragmento;
		//Shader de V�rtice
		idShaderVertice = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(idShaderVertice, 1, &codigoShaderVertice, NULL);
		glCompileShader(idShaderVertice);
		verificarErrores(idShaderVertice, "Vertice");
		//Shader de Fragmento
		idShaderFragmento = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(idShaderFragmento, 1, &codigoShaderFragmento, NULL);
		glCompileShader(idShaderFragmento);
		verificarErrores(idShaderVertice, "Fragmento");
		//Shader de Geometria
		GLuint idShaderGeometria;
		if (rutaShaderGeometria != "") {
			const char* codigoShaderGeometria = strCodigoShaderGeometria.c_str();
			idShaderGeometria = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(idShaderGeometria, 1, &codigoShaderGeometria, NULL);
			glCompileShader(idShaderGeometria);
			verificarErrores(idShaderGeometria, "Geometria");
		}
		//Programa de Shaders
		idPrograma = glCreateProgram();
		glAttachShader(idPrograma, idShaderVertice);
		glAttachShader(idPrograma, idShaderFragmento);
		if (rutaShaderGeometria != "") glAttachShader(idPrograma, idShaderGeometria);
		glLinkProgram(idPrograma);
		verificarErrores(idShaderVertice, "Programa");
		//Ahora ya podemos eliminar los programas de los shaders
		glDeleteShader(idShaderVertice);
		glDeleteShader(idShaderFragmento);
		if (rutaShaderGeometria != "") glDeleteShader(idShaderGeometria);
	}

	// Destructor
	~Shader() {
		glDeleteProgram(idPrograma);
	}

	// Activa el shader
	void usar() const {
		glUseProgram(idPrograma);
	}
	void usar(glm::mat4 ventanaVista, glm::mat4 ventanaProyeccion) const {
		glUseProgram(idPrograma);
		setMat4("vista", ventanaVista);
		setMat4("proyeccion", ventanaProyeccion);
	}
	//------------------------------------------------------------------------
	void setBool(const string& nombre, bool valor) const {
		glUniform1i(glGetUniformLocation(idPrograma, nombre.c_str()), (int)valor);
	}
	void setInt(const string& nombre, int valor) const {
		glUniform1i(glGetUniformLocation(idPrograma, nombre.c_str()), valor);
	}
	void setFloat(const string& nombre, float valor) const {
		glUniform1f(glGetUniformLocation(idPrograma, nombre.c_str()), valor);
	}
	//------------------------------------------------------------------------
	void setVec2(const string& nombre, float x, float y) const {
		glUniform2f(glGetUniformLocation(idPrograma, nombre.c_str()), x, y);
	}
	void setVec2(const string& nombre, const glm::vec2& valor) const {
		glUniform2fv(glGetUniformLocation(idPrograma, nombre.c_str()), 1, &valor[0]);
	}
	//------------------------------------------------------------------------
	void setVec3(const string& nombre, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(idPrograma, nombre.c_str()), x, y, z);
	}
	void setVec3(const string& nombre, const glm::vec3& valor) const {
		glUniform3fv(glGetUniformLocation(idPrograma, nombre.c_str()), 1, &valor[0]);
	}
	//------------------------------------------------------------------------
	void setVec4(const string& nombre, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(idPrograma, nombre.c_str()), x, y, z, w);
	}
	void setVec4(const string& nombre, const glm::vec4& valor) const {
		glUniform4fv(glGetUniformLocation(idPrograma, nombre.c_str()), 1, &valor[0]);
	}
	//------------------------------------------------------------------------
	void setMat2(const string& nombre, const glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation(idPrograma, nombre.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat3(const string& nombre, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(idPrograma, nombre.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const string& nombre, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(idPrograma, nombre.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};
