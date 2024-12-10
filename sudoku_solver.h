#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>

class SudokuSolver {
public:
    static constexpr int N = 9; // Size of the Sudoku board

    SudokuSolver(const std::vector<std::vector<int>>& fixed);
    void initializeBoard();
    int calculateCost() const;
    std::vector<int> swapInBlock(int blockRow, int blockCol);
    int computeMarkovChainLength() const;
    double computeInitialTemperature(int sampleSize);
    void simulatedAnnealing(double initialTemp, double alpha);
    void printBoard() const;

private:
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> fixedBoard;
    std::mt19937 generator;
};

#endif // SUDOKU_SOLVER_H


