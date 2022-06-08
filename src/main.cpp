#include "DateBase\DateBase.h"
#include "EscapeFromKapa.h"

int main(int argc, char* args[])
{
	IncludeWSADATA();
	char login[20] = { 0 };
	int format = LaunchGame(&gServer, &gClient);
	if (format != -1) {
		InitializeGameData((DataType)format);
		GameLoop((DataType)format);
	}
	return 0;
}