#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(){

    // create socket
    int socket_description = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_description == -1){
        cout << "Unable to create socket" << endl;
        return 1;
    }

    struct sockaddr_in server;
    struct sockaddr_in client;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8421);

    // bind to port
    if(bind(socket_description,(struct sockaddr*) &server, sizeof(server)) < 0){
        cout << "Unable to bind to port" << endl;
        return 1;
    }
    cout << "Socket bound, waiting for client..." << endl;

    // listen for client
    listen(socket_description, 3);

    int c = sizeof(struct sockaddr_in);
    int new_socket = accept(socket_description, (struct sockaddr*) &client, (socklen_t*)&c);
    if(new_socket < 0){
        cout << "Failed to accept client connection" << endl;
        return 1;
    }

    // TODO: Choose public base and mod

    // TODO: Choose a Server secret number

    // TODO: Raise base to secret number
    
    // TODO: Send to client

    // if( send(socket_description , &data, strlen(client_message) , 0) < 0)
	// {
	// 	cout << "Unable to send message to server";
	// 	return 1;
	// }

    // TODO: Wait for client response

    // int read_size;
    // while((read_size = recv(new_socket, client_message, 100, 0)) > 0){
    // }

    // TODO: Raise base to client response

    // if(read_size == 0){
    //     cout << "Client disconnected" << endl;
    // }
    // else if(read_size == -1){
    //     cout << "Server could not receive message from client" << endl;
    //     return 1;
    // }

    close(socket_description);

    return 0;
}