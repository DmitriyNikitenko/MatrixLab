#include <fstream>
#include <iostream>
#include<string>
#include "include/matrix.h"
#include "include/interpreter.h"

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Wrong amount arguments.Try again." << std::endl;
		return 1;
	}

	std::ifstream inFile(argv[1]);
	if (!inFile) {
		std::cerr << "Can not open " << argv[1] << ".Try again." << std::endl;
		return 2;
	}
	std::ofstream outFile(argv[2]);
	if (!outFile) {
		std::cerr << "Can not open " << argv[2] << ".Try again." << std::endl;
		inFile.close();
		return 2;
	}

	Interpreter inter(outFile);
	std::string input;
	int numberOfLine = 0;
	while (std::getline(inFile, input)) {
		++numberOfLine;
		if (input.find_first_not_of(" \t\r\n") == std::string::npos) {
			continue;
		}
		try {
			inter.executeLine(input);
		}
		catch (std::runtime_error e) {
			std::cerr << e.what();
		}
		catch (...) {
			outFile << "Error in line " << numberOfLine << "\n";
		}
	}

	inFile.close();
	outFile.close();
	return 0;
}