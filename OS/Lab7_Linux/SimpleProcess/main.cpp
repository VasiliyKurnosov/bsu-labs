#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[])
{
	sleep(1);
	std::cout << "Simple process is working..." << std::endl;
	return 0;
}
