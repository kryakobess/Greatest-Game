#include "DateBase\DateBase.h"
#include "EscapeFromKapa.h"

int main(int argc, char* args[])
{
	//IncludeWSADATA();
	//char login[20] = { 0 };
	//int format = LaunchGame(&gServer, &gClient);
	//if (format != -1) {
	//	InitializeGameData((DataType)format);
	//	GameLoop((DataType)format);
	//}
	InitializeGameData(HOST);
	char receive[1024] = "{SMP}[Login_character][520][st231]";
	char transmit[1024] = { 0 };
	char name[128]; char task[128]; char str[128];
	DataProcessing(receive, transmit);
	sscanf(transmit, "{%[A-Z ]}/%[a-zA-Z!_ ]/%s", task, name, str);
	//DataProcessing(transmit, receive);
	return 0;
}