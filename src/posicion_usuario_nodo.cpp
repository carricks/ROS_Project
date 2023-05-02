
//Nodo de introduccion de posicion de un usuario mediante sus coordenadas X, Y y Z.


// Librerias y paquetes
#include "ros/ros.h"
#include "interaccion/pos_usuario.h" // type: pos_usuario

// Macros auxiliares para definir los nombres del nodo y los mensajes y topics
#define NODE_NAME "posicion_usuario_nodo"
#define INFO_MSG_NAME "pos_usuario_topic"

// Uso de namespace para facilitar la programacion
using namespace std;
using namespace ros;

//Bucle principal

int main(int argc, char **argv){

    // Inicializacion del nodo Ros con los argumentos y el nombre proporcionados
    //Se ha definido el nombre del nodo en la variable NODE_NAME
    init(argc,argv, NODE_NAME);
    // Manejador del nodo
    NodeHandle node;

    ROS_INFO("Nodo de posicion registrado");

    // Publica mensajes para enviar la informacion al empaquetador
    Publisher publisher = node.advertise<interaccion::pos_usuario>(INFO_MSG_NAME, 0);

    // Duerme el nodo un segundo en cada iteracion
    Duration seconds_sleep(1);

    // Loop principal
    while (node.ok()){

        // Variables para introduccion de coordenadas
        int x;
        int y;
        int z;

        // Mensaje de tipo pos_usuario que almacena las coordenadas introducidas por el usuario
        interaccion::pos_usuario message;

        // Introducción por teclado de la coordenada X
        ROS_INFO("Introducir coordenada X:");
        cin >> x;

        // Introducción por teclado de la coordenada Y
        ROS_INFO("Introducir coordenada Y:");
        cin >> y;

        // Introducción por teclado de la coordenada Z
        ROS_INFO("Introducir coordenada Z:");
        cin >> z;

        // Se establecen los valores del mensaje con las coordenadas
        message.x = x;
        message.y = y;
        message.z = z;

        // Mensaje enviad por el publicador para que el Empaquetador lo trate
        publisher.publish(message);

        // Espera mensajes
        spinOnce();
        ROS_DEBUG ("Se duerme el nodo emisor durante un segundo");

        // El nodo se duerme durante un segundo
        seconds_sleep.sleep();

    }

    return 0;
}
