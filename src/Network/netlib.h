#ifndef  NETLIB
#define NETLIB
#define _CRT_SECURE_NO_WARNINGS
#define HAVE_STRUCT_TIMESPEC
#define MAXSIZECLENTS 50
#define MAX_DATA_SIZE 1024

#include <pthread.h>
#pragma comment(lib, "ws2_32.lib")
#include <stdbool.h>
#include <winsock.h> 
#include <stdio.h>
#include <Windows.h>

enum WSADATALIBSTATES
{
	SUCCESSFUL_INSTAL_WSADATA_LIB,
	ERROR_INSTAL_WSADATA_LIB
};
enum CLIENTSTATES
{
	SUCCESSFUL_CLIENT_INSTALATION,
	ERROR_CREATE_SOCKET,
	CONNECTION_CLOSED,
	CANT_SEND_MESSAGE
};
enum SERVERSTATES
{
	SUCCESSFUL_SERVER_INSTALATION,
	ERROR_CREATE_SERVER,
	CANT_START_SERVER,
	REACHED_MAX_NUMBER_CLIENTS,
	CLIENT_DISCONNECTED,
	ERROR_SENDING_DATA
};

typedef struct taskID {
	SOCKET socket;
	SOCKADDR_IN addr;
	void (*DataProcessing) (char*, char*);
}taskID;
typedef struct myServer {
	SOCKET serverSock;
	//taskID clients[MAXSIZECLENTS];
	int ClientCount;
	SOCKADDR_IN serverAddr;
	pthread_t serverThread;
	//обработка полученных и отправл€ющихс€ данных
	void (*DataProcessing) (char*, char*);
} myServer;
typedef struct myClient {
	SOCKET socket;
	SOCKADDR_IN serverAddr;
	pthread_t thread;
	char sentData[1024];
	char recievedData[1024];
	void (*DataAcceptance)(char*);
}myClient;
typedef struct PostData {
	char recieve[1024];
	char transmit[1024];
}PostData;

int IncludeWSADATA();
int StartServer(myServer* server, const char* ip, const int port);
int ConnectToServer(myClient* client, const char* ip, const int port);

#endif // ! NETLIB