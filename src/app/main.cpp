#include "application.h"

#include <stdlib.h>
#include <time.h>

/* WORKAROUND FOR (K)UBUNTU */
/* WRONG LINK TO PTHREAD CAUSES CRASH ON STARTUP IN C++11 MODE */
#include <pthread.h>
void* simpleFunc(void*) { return NULL; } void forcePThreadLink() { pthread_t t1; pthread_create(&t1, NULL, &simpleFunc, NULL); }
/* WORKAROUND FOR (K)UBUNTU */

//Main program
int main(int argc, char **argv)
{
	srand(time(NULL));

	if (!Application::getInstance()->init(argc, argv))
		return 1;

	Application::getInstance()->run();
	return 0;
}
