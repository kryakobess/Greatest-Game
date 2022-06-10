#include "DateBase\DateBase.h"
#include "EscapeFromKapa.h"

int main(int argc, char* args[])
{
	IncludeWSADATA();
	int format = LaunchGame();
	if (format != -1) {
		InitializeGameData((DataType)format);
		GameLoop((DataType)format);
	}
	return 0;
}