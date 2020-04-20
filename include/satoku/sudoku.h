#pragma once

#include <utility>
#include <stdexcept>
#include <vector>
#include <optional>

namespace satoku {

namespace detail {

    template <typename T>
    class SudokuBase {
    public:
        using element_type = T;
        using point_type = std::pair<std::size_t, std::size_t>;

        SudokuBase(std::int64_t size);
        SudokuBase(const std::vector<element_type>& data);
        ~SudokuBase() = default;

        SudokuBase(const SudokuBase&) = default;
        SudokuBase& operator=(const SudokuBase&) = default;

        SudokuBase(SudokuBase&&) = default;
        SudokuBase& operator=(SudokuBase&&) = default;

        element_type& operator[](point_type point) noexcept;
        const element_type& operator[](point_type point) const noexcept;

        std::int64_t getSize() const;
    private:
        std::vector<element_type> m_data;
        std::int64_t m_size;
    };

}

template class detail::SudokuBase<std::optional<std::int64_t>>;
using Sudoku = detail::SudokuBase<std::optional<std::int64_t>>;

template class detail::SudokuBase<std::int64_t>;
using SolvedSudoku = detail::SudokuBase<std::int64_t>;

}
