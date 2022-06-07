#include "DateBase\DateBase.h"
#include "EscapeFromKapa.h"



int main(int argc, char* args[])
{
	IncludeWSADATA();
	myServer server;
	int format = LaunchGame(&server, NULL);
	if (format != -1) {
		InitializeGameData((DataType)format);
		GameLoop((DataType)format);
	}
	return 0;
}