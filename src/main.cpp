// Support Code written by Michael D. Shah
// Last Updated: 6/10/21
// Please do not redistribute without asking permission.

// Functionality that we created
#include "SDLGraphicsProgram.hpp"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
	std::vector<std::string> functions;
	unsigned int lod = 200;
	float length = 10;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-lod") == 0)
		{
			lod = std::stoi(argv[i + 1]);
			i += 2;
		}
		else if (strcmp(argv[i], "-l") == 0)
		{
			length = std::stof(argv[i + 1]);
			i += 2;
		}
		else
		{
			functions.push_back(argv[i]);
		}
	}
	// Create an instance of an object for a SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(1280, 720, functions, lod, length);
	// Run our program forever
	mySDLGraphicsProgram.Loop();
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}
