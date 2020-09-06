#include "Routine.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace mumps;

static void printError(const std::string &file, const char *what)
{
	std::cerr << "error: Cannot " << what << " "<< file << std::endl;
	return;
}

Routine::Routine(const std::string &fname)
{
	mapFileToMemory(fname.c_str());
}
	
void Routine::readFromFile(const std::string &fname)
{
	mapFileToMemory(fname.c_str());
}

void Routine::mapFileToMemory(const char* fname)
{
	std::ifstream in(fname, std::ios_base::binary);
	if (in.fail()) {
		status = kFailure;
		printError(fname, "open");
		return;
	}
	in.seekg(0, std::ios_base::end);
	if (in.fail()) {
		status = kFailure;
		printError(fname, "seek");
		return;
	}
	size_t size = in.tellg();
	if (in.fail()) {
		status = kFailure;
		printError(fname, "tell");
		return;
	}
	in.seekg(0);
	if (in.fail()) {
		status = kFailure;
		printError(fname, "rewind");
		return;
	}
	source.resize(size);
	// load source to memory
	in.read(&source[0], size);	
	if (in.fail()) {
		status = kFailure;
		printError(fname, "read");
		return;
	}
	// successfull
	status = kMapped;
}

void Routine::dump() const
{
	std::cout << source << std::endl;
}
