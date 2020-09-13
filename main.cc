#include "Lexer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace mumps;

int main(int argc, char *argv[])
{
	std::vector<std::string> parameters;
	mumps::Routine routine;

	if (argc < 2) { 
		std::cerr << "./rmumps -s|--source filename\n";
		std::cerr << "./rmumps filename\n";
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "-s" || std::string(argv[i]) == "--source") {
			if (i + 1 < argc) {
				// map file to memory
				routine.readFromFile(argv[++i]);
				if (routine.fail())
					return -1;
			} else {
				std::cerr << "-s|--source options requires an argument" << std::endl;
				return -1;
			}
		} else {
			parameters.push_back(argv[i]);
		}
	}	
	
	if (parameters.size() != 0) {
		std::vector<mumps::Routine> routines;
		for (auto &p: parameters)
			routines.push_back(Routine(p));

		for (auto &r: routines) {
			mumps::Token tok;
			mumps::Lexer lexer(r);
			do {
				lexer.lexToken(tok);
				std::cerr << "Token: " << tok.getTypeName() << " " << tok.getValue() <<  std::endl;
			} while (tok.isNot(tok::eof));
		}
	}

	return 0;
}
