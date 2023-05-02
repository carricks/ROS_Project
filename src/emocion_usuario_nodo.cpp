/**
 Nodo para ingresar la emocion que percibe el usuario
**/

// Librerias y paquetes
#include "ros/ros.h"
#include "std_msgs/String.h"// type: std_msg/String

// Definicion de macros auxiliares para los nombres del nodo, los mensajes y topics utilizados
#define NODE_NAME "emocion_usuario_nodo"
#define INFO_MSG_NAME "emocion_topic"

// Uso de namespace para facilitar la programacion
using namespace std;
using namespace ros;


// Inicia operacion del nodo


int main(int argc, char **argv){

    // Se inicia nodo
    //Se ha definido el nombre del nodo en la variable NODE_NAME
    init(argc, argv, NODE_NAME);
    // Manejador del nodo ros
    NodeHandle node;

    ROS_INFO("Nodo de emocion registrado");

    // Publica mensajes para enviar la informacion al empaquetador
    Publisher publisher = node.advertise<std_msgs::String>(INFO_MSG_NAME, 0);

    // Variable para dormir el nodo un segundo en cada iteracion
    Duration seconds_sleep(1);

    // Bucle principal
    while (node.ok()){

        // Mensaje string que almacena la emocion y la envia al empaquetador
        std_msgs::String emocion;

        // Introducir por teclado de la emocion del usuario
        ROS_INFO("Introduzca la emocion del usuario:");
        cin >> emocion.data;

        // Envio de mensaje del publicador hacia el Empaquetador
        publisher.publish(emocion);

        // Esperar mensajes
        spinOnce();
        ROS_DEBUG ("Se duerme el nodo emisor durante un segundo");

        // El nodo se duerme durante un segundo antes de empezar la siguiente iteracion
        seconds_sleep.sleep();

    }

    return 0;
}		
