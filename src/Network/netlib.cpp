#include "netlib.h"

pthread_mutex_t clientLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t servLock = PTHREAD_MUTEX_INITIALIZER;
int Global_clientCount;

int IncludeWSADATA()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) == 0)
	{
		//Connected to socket lib
		return SUCCESSFUL_INSTAL_WSADATA_LIB;
	}
	else
	{
		return ERROR_INSTAL_WSADATA_LIB;
	}
}

void* ListenClient(void* arg) {
	taskID* clientTask = (taskID*)arg;
	while (true) {
		PostData data;
		int	ret = recv(clientTask->socket, data.recieve, 1024, 0);
		if (!ret || ret == SOCKET_ERROR)
		{
			return (void*)CLIENT_DISCONNECTED;
		}
		data.recieve[ret] = '\0';
		if (clientTask->DataProcessing != NULL)
		{
			clientTask->DataProcessing(data.recieve, data.transmit);
		}
		else
		{
			data.transmit[0] = 'O';
			data.transmit[1] = 'K';
			data.transmit[2] = '\0';
		}
		ret = send(clientTask->socket, data.transmit, strlen(data.transmit) + 1, 0);
		if (ret == SOCKET_ERROR)
		{
			return (void*)ERROR_SENDING_DATA;
		}
	}
	pthread_mutex_lock(&servLock);
	Global_clientCount--;
	free(clientTask);
	pthread_mutex_unlock(&servLock);
	closesocket(clientTask->socket);
}
//поток с циклом где сервер слушает
void* ServerThread(void* arg) {
	myServer* serverCfg = (myServer*)arg;
	if (bind(serverCfg->serverSock, (SOCKADDR*)&(serverCfg->serverAddr), sizeof(serverCfg->serverAddr)) == SOCKET_ERROR)
	{
		return (void*)CANT_START_SERVER;
	}
	listen(serverCfg->serverSock, MAXSIZECLENTS);
	int size = 0;
	while (true)
	{
		taskID* clientTask = (taskID*)malloc(sizeof(taskID));
		clientTask->DataProcessing = serverCfg->DataProcessing;
		if (Global_clientCount == MAXSIZECLENTS)
		{
			return (void*)REACHED_MAX_NUMBER_CLIENTS;
		}
		size = sizeof(clientTask->addr);
		clientTask->socket = accept(serverCfg->serverSock, (SOCKADDR*)&clientTask->addr, &size);
		if (clientTask->socket == INVALID_SOCKET)
		{
			//Error accept client
			continue;
		}
		else
		{
			//Client is accepted
			pthread_mutex_lock(&servLock);
			Global_clientCount++;
			pthread_mutex_unlock(&servLock);
		}
		pthread_t client;
		pthread_create(&client, NULL, ListenClient, (void*)clientTask);
		pthread_detach(client);
	}
	closesocket(serverCfg->serverSock);
	return 0;
}

int StartServer(myServer* server, const char* ip, const int port) {
	Global_clientCount = 0;
	SOCKET srvS = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	server->serverSock = srvS;
	if (server->serverSock == INVALID_SOCKET)
	{
		return ERROR_CREATE_SERVER;
	}
	server->serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	server->serverAddr.sin_family = AF_INET;
	server->serverAddr.sin_port = htons((u_short)port);
	pthread_t servThread;
	int status = pthread_create(&servThread, NULL, ServerThread, (void*)server);
	pthread_detach(servThread);
	return SUCCESSFUL_SERVER_INSTALATION;
}

//поток клиента где он отправляет инфу на сервер
void* SendToServer(void* arg) {
	myClient* client = (myClient*)arg;
	while (connect(client->socket, (SOCKADDR*)&(client->serverAddr), sizeof(client->serverAddr)) == SOCKET_ERROR)
	{
		//Can't connect to server
		Sleep(1000);
	}
	while(true)
	{
		if (strlen(client->sentData) != 0)
		{
			int ret = send(client->socket, client->sentData, strlen(client->sentData), 0);
			client->sentData[0] = '\0';
			if (ret == SOCKET_ERROR)
			{
				printf("Can't send message\n");
				closesocket(client->socket);
				return (void*)CONNECTION_CLOSED;
			}
			ret = recv(client->socket, client->recievedData, sizeof(client->recievedData), 0);
			if (ret == 0 || ret == WSAECONNRESET)
			{
				closesocket(client->socket);
				return (void*)CONNECTION_CLOSED;
			}
			if (ret < 0)
				continue;
			client->DataAcceptance(client->recievedData);
		}
	}
	closesocket(client->socket);
	return (void*)CONNECTION_CLOSED;
}
int ConnectToServer(myClient* client, const char* ip, const int port) {
	client->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client->socket == INVALID_SOCKET) {
		return ERROR_CREATE_SOCKET;
	}
	client->serverAddr.sin_family = AF_INET;
	client->serverAddr.sin_port = htons(port);
	client->serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	pthread_t clientThread;
	pthread_create(&clientThread, NULL, SendToServer, (void*)client);
	pthread_detach(clientThread);
	return SUCCESSFUL_CLIENT_INSTALATION;
}