#include <iostream>
#include "MainEngine.h"

MainEngine *mainEngine;

int main(int argc, char *argv[]) {
	std::cout << "QM Simulations by Michael Xu" << std::endl;
	std::cout << "2017-01-28" << std::endl << std::endl;

	mainEngine = new MainEngine;
	if (mainEngine->init == false)
		return -1;

	mainEngine->Loop();
	delete mainEngine;

	return 0;
}