#include "Iridium.h"

int main(int argc, char* argv[])
{
	Iridium iridium;

	std::vector<std::string> args;
	args.reserve(argc);

	for (int i = 0; i < argc; i++)
		args.emplace_back(argv[i]);

	iridium.run(args);
}
