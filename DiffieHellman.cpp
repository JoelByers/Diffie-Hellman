#include "DiffieHellman.h"
#include <cstdlib>
#include <time.h>
#include "fastmodexpon.h"
#include <sys/socket.h>
#include <iostream>

using namespace std;

DiffieHellman::DiffieHellman(int socket){
    this->socket = socket;
}

void DiffieHellman::serverGetPrivateKey(){
    // Choose public base and mod
    int mod = randPrime();
    int base = 3;
    // Choose a Server secret number
    srand(time(0));
    int serverSecret = rand() % 50;
    // Raise base to secret number
    int serverResult = FastModExpon(base, serverSecret, mod);
    // Send to client
    DiffieHellmanServerData data;
    data.base = base;
    data.mod = mod;
    data.serverResult = serverResult;

    if( send(this->socket , &data, sizeof(data), 0) < 0)
	{
		cout << "Unable to send server data to client";
		return;
	}

    // Wait for client response
    int clientResult = -1;
    recv(this->socket, &clientResult, sizeof(clientResult), 0) > 0;
    this->privateKey = FastModExpon(clientResult, serverSecret, mod);
}

void DiffieHellman::clientGetPrivateKey(){
    DiffieHellmanServerData serverData;

    recv(this->socket, &serverData, sizeof(serverData), 0);
    
    // Pick Client secret number
    srand(time(0) * 2);
    int clientSecret = rand() % 50;

    // Raise Client secret number to base and mod
    int clientResult = FastModExpon(serverData.base, clientSecret, serverData.mod);

    // Send Client result to server
    if(send(this->socket , &clientResult, sizeof(clientResult) , 0) < 0)
	{
		cout << "Unable to send client data to server" << endl;
		return;
	}

    // Raise Server result to Client secret number
    this->privateKey = FastModExpon(serverData.serverResult, clientSecret, serverData.mod);
}

int DiffieHellman::getPrivateKey(){
    return this->privateKey;
}