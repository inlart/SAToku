#include "satoku/satoku.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <optional>
#include <utility>
#include <tuple>

void printUsage(const std::string& name) {
	std::cout << name << " [size] [row],[column]=[value]" << std::endl;
}

std::optional<std::pair<std::string, std::string>> split(const std::string& str, char c) {
	auto pos = str.find(c);
	if(pos == std::string::npos) {
		return {};
	}
	return {std::pair<std::string, std::string>(str.substr(0, pos), str.substr(pos + 1))};
}

int main(int argc, char* argv[]) {

	if(argc < 2) {
		printUsage(argc > 0 ? argv[0] : "satoku-cli");
		return EXIT_FAILURE;
	}

	try {
		satoku::Sudoku sudoku(std::atoi(argv[1]));

		// -- set values
		for(int i = 2; i < argc; ++i) {
			std::string lhs, rhs;
			std::string tmp(argv[i]);

			auto eq = split(tmp, '=');
			if(!eq) {
				throw std::invalid_argument("Could not find '=' in " + tmp);
			}
			std::tie(lhs, rhs) = eq.value();

			std::int64_t value = std::stoi(rhs);

			auto comma = split(lhs, ',');
			if(!comma) {
				throw std::invalid_argument("Could not find ',' in " + lhs);
			}
			std::tie(lhs, rhs) = comma.value();

			std::int64_t row = std::stoi(lhs);
			std::int64_t column = std::stoi(rhs);

			sudoku[{row - 1, column - 1}] = value;
		}

		// -- solve
		satoku::SudokuSolver sudokuSolver(sudoku);
		auto solution = sudokuSolver.solve();

		// -- print
		if(solution) {
			auto sudokuSolution = solution.value();
			auto size = sudokuSolution.getSize();
			for(int i = 0; i < size; ++i) {
				for(int j = 0; j < size; ++j) {
					std::cout << sudokuSolution[{i, j}];
					if(j < size - 1) {
						std::cout << " ";
					}
				}
				std::cout << std::endl;
			}
		} else {
			std::cout << "No solution found" << std::endl;
		}
	}
	catch(std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
