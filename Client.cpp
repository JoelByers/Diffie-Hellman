#include <iostream>   
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include "fastmodexpon.h"
#include <unistd.h>
#include "SDES.h"

using namespace std;

struct DiffieHellmanServerData{
    int base;
    int mod;
    int serverResult;
};

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
    // Wait for Server to send public base, mod, and base raised to secret

    DiffieHellmanServerData serverData;

    recv(socket_description, &serverData, sizeof(serverData), 0);
    
    cout << "Received data from Server...\n";

    // Pick Client secret number
    srand(time(0) * 2);
    int clientSecret = rand() % 50;

    // Raise Client secret number to base and mod
    int clientResult = FastModExpon(serverData.base, clientSecret, serverData.mod);

    cout << "Received data from Server...\n";
    cout << "-------------------------------------\n";
    cout << "Base          : " << serverData.base << "\n";
    cout << "Mod           : " << serverData.mod << "\n";
    cout << "Client Secret : " << clientSecret << "\n";
    cout << "Server Result : " << serverData.serverResult << "\n";
    cout << "Client Result : " << clientResult << "\n";
    cout << "-------------------------------------\n";

    // Send Client result to Server
    cout << "Sending result to Server...\n";
    if(send(socket_description , &clientResult, sizeof(clientResult) , 0) < 0)
	{
		cout << "Unable to send client data to server" << endl;
		return 1;
	}

    // Raise Server result to Client secret number
    int privateKey = FastModExpon(serverData.serverResult, clientSecret, serverData.mod);
    bool key[10] = {0,0,0,0,0,0,0,0,0,0};
    asciiToBinary((char)privateKey, key);

    cout << "Private Key: " << privateKey << endl;

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