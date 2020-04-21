#pragma once

#include "sudoku.h"

#include <cryptominisat5/cryptominisat.h>

namespace satoku {

class SudokuSolver {
public:
	SudokuSolver(const Sudoku& sudoku);

	std::optional<SolvedSudoku> solve() noexcept;

private:
	void updateClauses();
	std::size_t getVariableNumber(std::pair<std::size_t, std::size_t> pos,
								  std::int64_t val) const noexcept;

	Sudoku m_sudoku;
	std::vector<SolvedSudoku> m_solutions;
	std::vector<std::vector<CMSat::Lit>> m_clauses;
};

} // namespace satoku
