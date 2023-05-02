/**
 EMPAQUETADOR es un paquete de interaccion global del sistema que reune la informacion introducida en los
 nodos anteriores y se encarga de mandarla a dialogo_nodo.
**/

// Librerias y paquetes
#include "ros/ros.h"
#include <string>
#include "interaccion/usuario.h" //type: usuario
#include "std_msgs/String.h" //type topic 1
#include "interaccion/inf_personal_usuario.h" //type topic 2
#include "interaccion/pos_usuario.h" //type topic 3



// Macros auxiliares para definir los nombres del nodo y los mensajes y topics utilizados
#define NODE_NAME "empaquetador_nodo" //nombre del nodo
#define INFO_MSG_NAME "user_topic" //topic
#define INFO_TOPIC_NAME_1 "emocion_topic" //recibe topic 1
#define INFO_TOPIC_NAME_2 "inf_pers_topic" //recibe topic 2
#define INFO_TOPIC_NAME_3 "pos_usuario_topic" //recibe topic 3

// Uso de namespace para facilitar la programacion
using namespace std;
using namespace ros;

// Mensaje de tipo usuario donde se reune la informacion recibida.
interaccion::usuario user;

// Flags auxiliares para saber la informacion que se ha recibido 
bool emotion_ready = false;
bool position_ready = false;
bool information_ready = false;

/**
 funcionCallbackEmotionTopic se ejecuta cuando se recibe el mensaje procedente
 del NODO EMOCION con la informacion proporcionada por el usuario.
**/
void funcionCallbackEmotionTopic(const std_msgs::String::ConstPtr& msg){

	ROS_INFO("Emocion: %s", msg->data.c_str());
        // Se actualiza el contenido del mensaje
        user.emocion = msg->data.c_str();
        // Se actualiza el flag
        emotion_ready = true;
}

/**
 funcionCallbackInfPersonalTopic se ejecuta cuando se recibe el mensaje procedente
 del NODO DE INFO PERSONAL con la informacion dada por el usuario
**/
void funcionCallbackInfPersonalTopic(const interaccion::inf_personal_usuario::ConstPtr& msg){

	ROS_INFO("Nombre: %s", msg->nombre.c_str());
	ROS_INFO("Edad: %d", msg->edad);
	for (int i = 0; i < msg->idiomas.size(); ++i){
		ROS_INFO("Idiomas: %s", msg->idiomas[i].c_str());
	}

	user.infPersonal = *msg; // Actualizamos el contenido del mensaje
	information_ready = true; // Actualizamos el flag
}

//Funcion MAIN

void funcionCallbackPositionTopic(const interaccion::pos_usuario::ConstPtr& msg){

	ROS_INFO("Coordenada X: %d", msg->x);
	ROS_INFO("Coordenada Y: %d", msg->y);
	ROS_INFO("Coordenada Y: %d", msg->z);

        // Se actualiza el contenido del mensaje
        user.posicion = *msg;
        // Se actualiza el flag
        position_ready = true;

}

/**
 fuctionCallbackShowMessage se ejecuta cuando se recibe el mensaje procedente
 de EMPAQUETADOR con la informacion proporcionada por el usuario
**/


int main(int argc, char **argv){
	
        // Inicializacion del nodo
        //Se ha definido el nombre del nodo en la variable NODE_NAME
	init(argc,argv, NODE_NAME);
        // Manejador del nodo ros
        NodeHandle node;

        ROS_INFO("Nodo empaquetador registrado");

        // Suscriptores a los posibles mensajes de entrada procedentes delos nodos de emocion, posicion e informacion
	Subscriber subscriptor1 = node.subscribe(INFO_TOPIC_NAME_1, 0, funcionCallbackEmotionTopic);
	Subscriber subscriptor2 = node.subscribe(INFO_TOPIC_NAME_2, 0, funcionCallbackInfPersonalTopic);
	Subscriber subscriptor3 = node.subscribe(INFO_TOPIC_NAME_3, 0, funcionCallbackPositionTopic);
	
        // Publica mensajes para mandar la informacion al NODO DIALOGO
	Publisher publisher = node.advertise<interaccion::usuario>(INFO_MSG_NAME, 0);


        // Duerme el nodo un segundo en cada iteracion
	Duration seconds_sleep(1);

        // Bucle principal

	while (node.ok()){

		// Comprobacion de flags para ver si toda la informacion está lista
		if (emotion_ready && information_ready && position_ready){

                        // Reset de los flags
			emotion_ready = false;
			position_ready = false;
			information_ready = false;
			publisher.publish(user); // Publicacion del mensaje
		}

                // Funcion que espera a posibles mensajes o servicios
		spinOnce();

                // El nodo se duerme durante un segundo
		seconds_sleep.sleep();
		
	}	

	return 0;
}		
