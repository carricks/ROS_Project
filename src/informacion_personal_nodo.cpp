/**
 NODO DE INFORMACION PERSONAL: permite la introduccion de los datos de un usuario mediante teclado.
**/

// Librerias y paquetes
#include "ros/ros.h"
#include <iostream>
#include <string>
#include <vector>
#include "interaccion/inf_personal_usuario.h" // type: inf_personal_usuario

// Macros auxiliares para definir los nombres del nodo y los mensajes y topics utilizados
#define NODE_NAME "informacion_personal_nodo"
#define INFO_MSG_NAME "inf_pers_topic"

// Uso de namespace para facilitar la programacion
using namespace std;
using namespace ros;

/**
 Funcion MAIN y parametros.
 **/

int main(int argc, char **argv){

    // Inicializacion del nodo Ros con los argumentos y el nombre proporcionados
    //Se ha definido el nombre del nodo en la variable NODE_NAME
    init(argc,argv, NODE_NAME);

    // Manejador del nodo ros
    NodeHandle node;

    ROS_INFO("Nodo de informacion registrado");

    // Publicador de mensajes para mandar la informacion introducida al Empaquetador
    Publisher publisher = node.advertise<interaccion::inf_personal_usuario>(INFO_MSG_NAME, 0);

    // Variable para dormir el nodo un segundo en cada iteracion
    Duration seconds_sleep(1);

    // Bucle principal
    while (node.ok()){

        // Variables auxiliares para la introduccion de los datos
        string nombre;
        string aux;

        int edad;
        int numIdiomas;
        vector<string> idiomas;

        // Mensaje de tipo inf_personal_usuario que almacena los datos y los envia al Empaquetador
        interaccion::inf_personal_usuario message;

        // Ingreso nombre de usuario por teclado
        ROS_INFO("Introducir solo el nombre del Usuario ");
        cin >> nombre;

        // Ingreso de edad por teclado
        ROS_INFO("Introducir la edad del usuario");
        cin >> edad;

        // Ingreso de Numero de idiomas
        ROS_INFO("Cantidad de idiomas que hablas:");
        cin >> numIdiomas;

        // Bucle para rellenar el vector de idiomas
        int i;
        for (i = 0; i<numIdiomas; i++){
            ROS_INFO("Introducir el idioma %d", i+1);
            cin >> aux;
            idiomas.push_back(aux);
        }

        // Se establecen los valores introducidos
        message.nombre = nombre;
        message.edad = edad;
        message.idiomas = idiomas;

        // El publicador envia mensajes para que el empaquetador lo trate
        publisher.publish(message);

        // Funcion necesaria para ejecutar funciones a la vez que el nodo se queda
        // esperando a posibles mensajes o servicios
        spinOnce();
        ROS_DEBUG ("Dormir el nodo de informacion durante un segundo");

        // El nodo se duerme durante un segundo antes de empezar la siguiente iteracion
        seconds_sleep.sleep();
    }

    return 0;
}
