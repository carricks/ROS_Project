/**
 NODO que implementa un servicio para multiplicar la edad introducida por 2.
**/

// Librerias y paquetes
#include "ros/ros.h"
#include "interaccion/multiplicador.h"

// Macros auxiliares
#define NODE_NAME "matematico_nodo" //nombre del nodo
#define INFO_MSG_NAME "user_topic" // topic
#define SRV_NAME "multiplication_service" //type srv

// Uso de namespace para facilitar la programacion
using namespace std;
using namespace ros;

// multiplicationService invoca al servicio desde dialogo nodo.

bool multiplicationService(interaccion::multiplicador::Request &req, interaccion::multiplicador::Response &res){

	res.resultado = req.entrada * 2;

	ROS_INFO("Peticion: x = %d", (int)req.entrada);

	ROS_INFO("Respuesta: %d", (int)res.resultado);

	return true;

}


 //MAIN se ejecuta al iniciar el nodo


int main(int argc, char **argv){

        //Se ha definido el nombre del nodo en la variable NODE_NAME
	init(argc, argv, NODE_NAME);

        // Manejador del nodo
        NodeHandle node;

	// Servidor que proporciona el servicio de multiplicacion que se queda escuchando peticiones
	ServiceServer service = node.advertiseService(SRV_NAME,multiplicationService);

        ROS_INFO("Nodo Matematico registrado");

	// Funcion que bloquea el nodo esperando a que se realice una llamada al servicio
	spin();

	return 0;

}
