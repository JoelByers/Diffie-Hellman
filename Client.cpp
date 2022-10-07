#include <iostream>   
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
using namespace std;

int main(){

    // create socket
    int socket_description = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_description == -1){
        cout << "Unable to create socket" << endl;
        return 1;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("0.0.0.0");
	server.sin_family = AF_INET;
    server.sin_port = htons(8421);

	if (connect(socket_description , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
        cout << "Unable to connect" << endl;
		return 1;
	}

    // TODO: Wait for Server to send public base, mod, and base raised to secret

    // while((read_size = recv(new_socket, client_message, 100, 0)) > 0){
    // }

    // TODO: Pick Client secret number

    // TODO: Raise Client secret number to base and mod

    // TODO: Send Client result to Server

    // TODO: Raise Server result to Client secret number

    // Not sure if this is needed
    // close(socket_description);
}