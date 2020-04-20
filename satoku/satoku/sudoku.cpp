#include "satoku/sudoku.h"

#include <cmath>
#include <cryptominisat5/cryptominisat.h>

namespace {
    bool isPerfectSquare(std::int64_t number) {
        std::int64_t temp = std::sqrt(number);
        return temp * temp == number;
    }
}

namespace satoku {

namespace detail {

template <typename T>
SudokuBase<T>::SudokuBase(std::int64_t size) : m_data(size * size), m_size(size) {
    if(!isPerfectSquare(size)) {
        throw std::invalid_argument("Sudoku size has to be a perfect square");
    }
}

template <typename T>
SudokuBase<T>::SudokuBase(const std::vector<element_type>& data) : m_data(data), m_size(std::sqrt(data.size())) {
    if(!isPerfectSquare(m_data.size()) || !isPerfectSquare(m_size)) {
        throw std::invalid_argument("Sudoku size has to be a perfect square");
    }
}

template <typename T>
std::int64_t SudokuBase<T>::getSize() const {
    return m_size;
}

template <typename T>
auto SudokuBase<T>::operator[](point_type point) noexcept -> element_type& {
    return m_data[point.first * getSize() + point.second];
}

template <typename T>
auto SudokuBase<T>::operator[](point_type point) const noexcept -> const element_type& {
    return m_data[point.first * getSize() + point.second];
}

}
}
