#include "application.h"

#include <stdlib.h>
#include <time.h>

//Main program
int main(int argc, char **argv)
{
	srand(time(NULL));
	if (!Application::getInstance()->init(argc, argv))
		return 1;

	Application::getInstance()->run();
	return 0;
}
