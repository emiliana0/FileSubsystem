#include "Terminal.h"
int main() {

    Terminal terminal;

    try {
    terminal.run();
	}
	catch (std::invalid_argument& exc)
	{
		std::cout << "Invalid command: " << exc.what() << std::endl;
	}
	catch (std::exception& exc)
	{
		std::cout << exc.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown error" << std::endl;
	}
	return 0;
}