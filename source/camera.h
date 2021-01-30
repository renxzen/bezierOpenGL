#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Define las posibles opciones para el movimiento de camara
enum movimientoCamara {
    ADELANTE,
    ATRAS,
    IZQUIERDA,
    DERECHA,
    ARRIBA,
    ABAJO
};

// Valores por defecto
const float Yaw = -90.0;
const float Pitch = 0.0;
const float Velocidad = 2.5;
const float Sensibilidad = 0.1;
const float Zoom = 45.0;

class Camera {
private:
    // Calcula el vector del frente de los angulos de la camara
    void updateCameraVectors() {
        // Calcula el nuevo vector de frente
        glm::vec3 Frente;
        Frente.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        Frente.y = sin(glm::radians(pitch));
        Frente.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        frente = glm::normalize(Frente);
        // Tambien re-calcula los vectores derecha y arriba
        derecha = glm::normalize(glm::cross(frente, mundoArriba));  // normaliza los vectores, porque su longitud se acerca a 0 cuando se mira arriba o abajo lo cual resulta en un movimiento mas lento
        arriba = glm::normalize(glm::cross(derecha, frente));
    }
public:
    // Atributos de camara
    glm::vec3 posicion;
    glm::vec3 frente;
    glm::vec3 arriba;
    glm::vec3 derecha;
    glm::vec3 mundoArriba;

    // Angulos de Euler
    float yaw;
    float pitch;

    // Opciones de camara
    float velocidadMovimiento;
    float sensibilidadMouse;
    float zoom;

    // Logica de tiempo
    float tiempoDelta = 0.0; // Tiempo entre el frame actual y el ultimo
    float ultimoFrame = 0.0;

    // Auxiliares de inicio
    bool primerMouse = false;
    bool fps = false;
    float lastX;
    float lastY;

    // Constructor con vectores
    Camera(glm::vec3 _posicion = glm::vec3(0.0, 0.0, 0.0), glm::vec3 _arriba = glm::vec3(0.0, 1.0, 0.0), float _yaw = Yaw, float _pitch = Pitch)
        : frente(glm::vec3(0.0, 0.0, -1.0)), velocidadMovimiento(Velocidad), sensibilidadMouse(Sensibilidad), zoom(Zoom) {
        posicion = _posicion;
        mundoArriba = _arriba;
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();
    }

    // Constructor con valores escalares
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float _yaw, float _pitch)
        : frente(glm::vec3(0.0, 0.0, -1.0)), velocidadMovimiento(Velocidad), sensibilidadMouse(Sensibilidad), zoom(Zoom) {
        posicion = glm::vec3(posX, posY, posZ);
        mundoArriba = glm::vec3(upX, upY, upZ);
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();
    }

    // Retorna la matriz de vista calculada utilizando los angulos de Euler y la Matriz LookAt
    glm::mat4 getMatrizVista() {
        // Propia funcion de LookAt
        glm::vec3 objetivo = posicion + frente;

        // 1. Position = conocida
        // 2. Calcular direccion de camara
        glm::vec3 zEje = glm::normalize(posicion - objetivo);
        // 3. Obtener el vector del eje positivo derecho
        glm::vec3 xEje = glm::normalize(glm::cross(glm::normalize(mundoArriba), zEje));
        // 4. Calcular el vector de la camara arriba
        glm::vec3 yEje = glm::cross(zEje, xEje);

        // Crear matrices de traslacion y rotacion
        // En GLM accesamos elementos como mat[col][row] por el formato columna-fila
        glm::mat4 translacion = glm::mat4(1.0f); // Identificar con matriz por defecto
        translacion[3][0] = -posicion.x; // Tercera columna, primera fila
        translacion[3][1] = -posicion.y;
        translacion[3][2] = -posicion.z;
        glm::mat4 rotacion = glm::mat4(1.0f);
        rotacion[0][0] = xEje.x; // Primera columna, primera fila
        rotacion[1][0] = xEje.y;
        rotacion[2][0] = xEje.z;
        rotacion[0][1] = yEje.x; // Primera columna, segunda fila
        rotacion[1][1] = yEje.y;
        rotacion[2][1] = yEje.z;
        rotacion[0][2] = zEje.x; // Primera columna, tercera fila
        rotacion[1][2] = zEje.y;
        rotacion[2][2] = zEje.z;

        // Devolver la matriz LookAt como la combinacion de las matrices de traslacion y rotacion
        return rotacion * translacion; // Recuerda leer de derecha a izquierda (primero traslacion luego rotacion)

        // Adicionalmente se puede utilizar la funcion que la libreria GLM te proporciona
        // return glm::lookAt(posicion, posicion + frente, arriba);
    }

    // ----------------------------------------------------------------------------------------------------------------
    // INPUT 
    // Procesa input recibido por el teclado segun el ENUM
    void procesarTeclado(movimientoCamara direccion) {
        float velocidad = velocidadMovimiento * tiempoDelta;
        if (direccion == ADELANTE) posicion += frente * velocidad;
        if (direccion == ATRAS) posicion -= frente * velocidad;
        if (direccion == IZQUIERDA) posicion -= derecha * velocidad;
        if (direccion == DERECHA) posicion += derecha * velocidad;
        if (direccion == ARRIBA) posicion += glm::vec3(0.0, velocidad, 0.0);
        if (direccion == ABAJO) posicion -= glm::vec3(0.0, velocidad, 0.0);

        // Se asegura que el usuario se mantenga en el nivel del suelo
        if (fps) posicion.y = 0.0;
    }

    // Procesa input recibido por el movimiento mouse. Se espera el valor offset en ambas direcciones x y y
    void procesarMovMouse(float xoffset, float yoffset, GLboolean restringirPitch = true) {
        xoffset *= sensibilidadMouse;
        yoffset *= sensibilidadMouse;

        yaw += xoffset;
        pitch += yoffset;

        // Asegurarse que cuando el  pitch este fuera de limites, la ventana no se de la vuelta
        if (restringirPitch) {
            if (pitch > 89.0) pitch = 89.0;
            if (pitch < -89.0) pitch = -89.0;
        }

        // Actualiza los vectores frente, derecha y arriba usando los angulos actualizados de Euler
        updateCameraVectors();
    }

    // Procesa input recibido por la rueda del mouse.  Solo requiere el input del eje vertical de la rueda
    void procesarRuedaMouse(float yoffset) {
        zoom -= (float)yoffset;
        if (zoom < 1.0) zoom = 1.0;
        if (zoom > 45.0) zoom = 45.0;
    }

};