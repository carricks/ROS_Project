/**
 NODO RELOJ se encarga de imprimir la hora local y la hora UTC a una frecuencia de 3 veces
 por segundo junto con los segundos transcurridos desde que se ha recibido el mensaje de start o reset.
 Tambien usa un ros::Timer para notificar al NODO DIALOGO que el nodo sigue activo cada 60
 segundos.

**/

// Librerias y paquetes
#include "ros/ros.h"
#include <string>
#include <ctime>
#include "std_msgs/String.h" //topic start y reset
#include "std_msgs/Bool.h" //topic de still_alive
#include "boost/date_time/posix_time/posix_time.hpp" //Hora: UTC & Local

// Macros auxiliares
#define NODE_NAME "reloj_nodo"//nombre del nodo
#define RELOJ_MSG_NAME "still_alive" //topic
#define RESET_TOPIC_NAME "reset_topic" //topic
#define START_TOPIC_NAME "start_topic"//topic
#define COUNTDOWN_TIME 60 //Timer que envia un msd de still_alive una vez transcurrido los 60 s

// Uso de namespace para facilitar la programacion
using namespace std;
using namespace ros;
using namespace boost::posix_time;

// Variable Time que almacena el valor del tiempo en el momento que llega un mensaje
Time startTime;

// Flag necesario para empezar a mostrar la hora solo cuando se haya recibido el primer mensaje
bool clock_start = false;

// Segundos transcurridos desde que se ha recibido el primer mensaje
int totalSeconds = 0;

// Publicador de mensajes que manda el mensaje still_alive
Publisher publisher;

/**
 timerCallback se ejecuta cada vez que se vence el tiempo indicado por el timer.
 Cuando se vence, se envia el mensaje de still_alive al que esta suscrito el dialogo.
**/


void timerCallback(const ros::TimerEvent&){
	std_msgs::Bool still_alive;
	still_alive.data = true;
	publisher.publish(still_alive);
}

/**
 printClock se encarga de imprimir la hora local y la hora UTC.
 Adicionalmente, imprime los segundos transcurridos desde que se recibio el mensaje de start/reset.
**/

void printClock(){

        // Segundos desde el inicio hasta el momento actual
        totalSeconds = (Time::now() - startTime).toSec();

        // Declaracion de una variable de tiempo local
        ptime t_local(second_clock::local_time());

        // Declaracion de una variable de tiempo UTC
        ptime t_utc(second_clock::universal_time());

    ROS_INFO("HORA LOCAL: %s", to_simple_string(t_local).c_str());
    ROS_INFO("HORA UTC: %s", to_simple_string(t_utc).c_str());

    ROS_INFO("SEGUNDOS DESDE EL INICIO AL FINAL: %lf", (double)(Time::now() - startTime).toSec());

}

/**
 fuctionCallbackReset se ejecuta cada vez que se recibe un mensaje de reset desde NODO DIALOGO
 Resetea la variable usada para contar los segundos que han
 transcurrido y activa el flag de impresión de hora.
**/

void funcionCallbackReset(const std_msgs::String::ConstPtr& msg){

    ROS_INFO("MENSAJE RESET: %s", msg->data.c_str());
	startTime = Time::now();
	clock_start = true;
}

/**
 fuctionCallbackStart se ejecuta cada vez que se recibe un mensaje de start desde NODO DIALOGO
 Resetea la variable usada para contar los segundos que han transcurrido y activa el flag de impresión de hora.
**/

void funcionCallbackStart(const std_msgs::String::ConstPtr& msg){

    ROS_INFO("MENSAJE DE INICIO: %s", msg->data.c_str());
	startTime = Time::now();
	clock_start = true;
}


// MAIN se ejecuta cuando se inicia el NODO


int main(int argc, char **argv){
	
        // Inicializacion del nodo ROS
        //Se ha definido el nombre del nodo en la variable NODE_NAME
	init(argc,argv, NODE_NAME);

        // Manejador del nodo ros
        NodeHandle node;
 
        ROS_INFO("Nodo Reloj registrado");

        // Suscriptores a los posibles mensajes del NODO DIALOGO
	Subscriber subscriptor1 = node.subscribe(START_TOPIC_NAME, 0, funcionCallbackStart);
	Subscriber subscriptor2 = node.subscribe(RESET_TOPIC_NAME, 0, funcionCallbackReset);
	
	// Timer que se vence cada 60 segundos para indicar que el nodo sigue activo
	Timer timer = node.createTimer(Duration(COUNTDOWN_TIME), timerCallback);

	// Publicador de mensajes para indicar que el nodo sigue activo
	publisher = node.advertise<std_msgs::Bool>(RELOJ_MSG_NAME, 0);

        // indicar la frecuencia con la que se tiene que ejecutar el bucle principal
	Rate rate(3);

	// Bucle principal
	while (node.ok()){

		// Impresion de hora y segundos si el flag esta activo
		if (clock_start){
			printClock();
		}

                // Funcion que ejecuta otras funciones a la vez que el nodo se queda espera mensajes y servicios

		spinOnce();
		ROS_DEBUG ("Se duerme el nodo emisor durante un segundo");

		// Duerme el nodo durante el tiempo necesario para cumplir la frecuencia indicada
		rate.sleep();
		
	}	

	return 0;
}		
