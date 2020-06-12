# SAToku

SAToku is a C++ library to solve sudoku puzzles. It transforms the problem to a SAT problem and utilizes cryptominisat to solve the problem.
SAToku supports different sizes (e.g. 4x4, the classic 9x9, 16x16) as long as they are perfect squares.

## Dependencies

* cryptominisat5
* meson (build)
* ninja (build)
* compiler with C++17 support

## Library

The code shown uses SAToku to solve a 4x4 Sudoku with the entries
```
-----------------
| 3 |   | 4 |   |
|---+---+---+---|
|   | 1 |   | 2 |
|---+---+---+---|
|   | 4 |   | 3 |
|---+---+---+---|
| 2 |   | 1 |   |
-----------------
```

```cpp
#include "satoku/satoku.h"

int main(int argc, char* argv[]) {
	satoku::Sudoku sudoku({
		3, {}, 4, {},
		{}, 1, {}, 2,
		{}, 4, {}, 3,
		2, {}, 1, {}
	});

    // -- find a solution
	satoku::SudokuSolver sudokuSolver(sudoku);
	auto solution = sudokuSolver.solve();

	if(solution) {
		auto sudokuSolution = solution.value();
		auto size = sudokuSolution.getSize();
        // -- print the solution
		for(int i = 0; i < size; ++i) {
			for(int j = 0; j < size; ++j) {
				std::cout << sudokuSolution[{i, j}];
				if(j < size - 1) {
					std::cout << " ";
				}
			}
			std::cout << std::endl;
		}
	}
	else {
		std::cout << "No solution found" << std::endl;
	}

	return EXIT_SUCCESS;
}
```

## CLI

The `satoku-cli` takes the Sudoku size as first argument followed by arguments setting values in the format `[row],[column]=[value]`.
Note that the first row and column starts at 1 and not 0.

To solve the same Sudoku as in the Library section you can call `satoku-cli` with
```sh
satoku-cli 4 1,1=3 1,3=4 2,2=1 2,4=2 3,2=4 3,4=3 4,1=2 4,3=1
```
