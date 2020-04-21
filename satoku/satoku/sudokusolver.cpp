#include "satoku/sudokusolver.h"

#include <cmath>

namespace satoku {

SudokuSolver::SudokuSolver(const Sudoku& sudoku) : m_sudoku(sudoku) {
	updateClauses();
}

std::size_t SudokuSolver::getVariableNumber(std::pair<std::size_t, std::size_t> pos,
											std::int64_t val) const noexcept {
	auto size = m_sudoku.getSize();
	return (pos.first * size + pos.second) * size + val;
}

void SudokuSolver::updateClauses() {
	std::vector<CMSat::Lit> clause;

	auto size = m_sudoku.getSize();

	// -- at least one number
	for(int i = 0; i < size; ++i) {
		for(int j = 0; j < size; ++j) {
			clause.clear();
			for(int k = 0; k < size; ++k) {
				clause.push_back(CMSat::Lit(getVariableNumber({i, j}, k), false));
			}
			m_clauses.push_back(clause);
		}
	}

	// -- at most one number
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; ++j) {
			for(int k = 0; k < size; k++) {
				for(int l = k + 1; l < size; l++) {
					clause.clear();
					clause.push_back(CMSat::Lit(getVariableNumber({i, j}, k), true));
					clause.push_back(CMSat::Lit(getVariableNumber({i, j}, l), true));
					m_clauses.push_back(clause);
				}
			}
		}
	}

	// -- one in a row
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; ++j) {
			for(int k = 0; k < size; k++) {
				for(int l = k + 1; l < size; l++) {
					clause.clear();
					clause.push_back(CMSat::Lit(getVariableNumber({i, k}, j), true));
					clause.push_back(CMSat::Lit(getVariableNumber({i, l}, j), true));
					m_clauses.push_back(clause);
				}
			}
		}
	}

	// -- one in a column
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			for(int k = 0; k < size; k++) {
				for(int l = k + 1; l < size; l++) {
					clause.clear();
					clause.push_back(CMSat::Lit(getVariableNumber({k, i}, j), true));
					clause.push_back(CMSat::Lit(getVariableNumber({l, i}, j), true));
					m_clauses.push_back(clause);
				}
			}
		}
	}

	// -- one in a block
	std::int64_t blockLength = std::sqrt(size);
	for(int a = 0; a < size; a += blockLength) {
		for(int b = 0; b < size; b += blockLength) {
			//[a,b] is the first element in the block
			for(int num = 0; num < size; num++) {
				for(int i = a; i < a + blockLength; i++) {
					for(int j = b; j < b + blockLength; j++) {
						for(int k = a; k < a + blockLength; k++) {
							for(int l = b; l < b + blockLength; l++) {
								if(i == k || j == l)
									continue;
								clause.clear();
								clause.push_back(CMSat::Lit(getVariableNumber({i, j}, num), true));
								clause.push_back(CMSat::Lit(getVariableNumber({k, l}, num), true));
								m_clauses.push_back(clause);
							}
						}
					}
				}
			}
		}
	}

	// -- set values
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			auto val = m_sudoku[{i, j}];
			if(val) {
				clause.clear();
				clause.push_back(CMSat::Lit(getVariableNumber({i, j}, val.value() - 1), false));
				m_clauses.push_back(clause);
			}
		}
	}
}

std::optional<SolvedSudoku> SudokuSolver::solve() noexcept {
	CMSat::SATSolver solver;
	solver.new_vars(getVariableNumber({m_sudoku.getSize(), 0}, 0));

	for(auto&& clause : m_clauses) {
		solver.add_clause(clause);
	}

	auto ret = solver.solve();

	if(ret == CMSat::l_True) {
		auto size = m_sudoku.getSize();
		SolvedSudoku result(size);

		for(int i = 0; i < size; ++i) {
			for(int j = 0; j < size; ++j) {
				for(int k = 0; k < size; ++k) {
					if(solver.get_model()[getVariableNumber({i, j}, k)] == CMSat::l_True) {
						result[{i, j}] = k + 1;
					}
				}
			}
		}

		m_solutions.push_back(result);
		return result;
	}

	return {};
}

} // namespace satoku
