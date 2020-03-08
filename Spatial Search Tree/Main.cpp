#include "Program.h"

int main(int argc, char** argv)
{
	sf::err().rdbuf(NULL);
	Program p(1000, 500);
	return p.mainLoop();
}