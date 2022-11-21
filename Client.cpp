#include <iostream>   
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include "fastmodexpon.h"
#include <unistd.h>
#include "SDES.h"
#include "DiffieHellman.h"

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

    cout << "Connected to Server...\n";

    DiffieHellman diffieHellman(socket_description);
    diffieHellman.clientGetPrivateKey();
    bool key[10] = {0,0,0,0,0,0,0,0,0,0};
    asciiToBinary((char)diffieHellman.getPrivateKey(), key);

    cout << "Private Key: " << diffieHellman.getPrivateKey() << endl;

    bool encryptedBytes[100][8] = {{}};
    recv(socket_description, &encryptedBytes, sizeof(encryptedBytes), 0);

    // Decrypt
    string message;
    for(int i = 0; i < 100; i++){
        decrypt(encryptedBytes[i], key);
        char c = binaryToChar(encryptedBytes[i]);
        message.push_back(c);
    }

    cout << message;
    close(socket_description);

    return 0;
}