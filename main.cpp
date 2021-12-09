#include <Windows.h>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	EXECUTION_STATE prevES = SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);

	std::string input;
	bool running = true;

	while (running)
	{
		std::cout << "Keep alive active... press 'q' and enter to exit" << std::endl;
		std::cin >> input;

		if (input == "q" || input == "Q")
		{
			running = false;
		}
	}

	SetThreadExecutionState(prevES);

	std::cout << "Keep alive stopped!" << std::endl;

	return 0;
}