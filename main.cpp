#include "sudoku_solver.h"

int main() {
    // Read the fixed Sudoku board
    std::vector<std::vector<int>> fixedBoard(SudokuSolver::N, std::vector<int>(SudokuSolver::N));
    for (int i = 0; i < SudokuSolver::N; i++) {
        for (int j = 0; j < SudokuSolver::N; j++) {
            std::cin >> fixedBoard[i][j];
        }
    }

    // Create the solver instance and solve the puzzle
    SudokuSolver solver(fixedBoard);
    solver.initializeBoard();
    double initialTemp = solver.computeInitialTemperature(10);
    solver.simulatedAnnealing(initialTemp, 0.991);
    solver.printBoard();

    return 0;
}
