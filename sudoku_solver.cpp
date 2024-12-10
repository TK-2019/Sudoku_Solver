#include "sudoku_solver.h"

SudokuSolver::SudokuSolver(const std::vector<std::vector<int>>& fixed)
    : fixedBoard(fixed), generator(std::random_device{}()) {
    board.resize(N, std::vector<int>(N, 0)); // Initialize the board with zeros
}

void SudokuSolver::initializeBoard() {
    for (int blockRow = 0; blockRow < 3; blockRow++) {
        for (int blockCol = 0; blockCol < 3; blockCol++) {
            std::vector<int> availableNumbers;
            for (int i = 1; i <= N; i++) availableNumbers.push_back(i);

            // Remove fixed numbers from the block
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int r = blockRow * 3 + i, c = blockCol * 3 + j;
                    if (fixedBoard[r][c] != 0) {
                        availableNumbers.erase(
                            std::remove(availableNumbers.begin(), availableNumbers.end(), fixedBoard[r][c]),
                            availableNumbers.end());
                    }
                }
            }

            // Shuffle and fill the block with available numbers
            std::shuffle(availableNumbers.begin(), availableNumbers.end(), generator);
            int idx = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int r = blockRow * 3 + i, c = blockCol * 3 + j;
                    board[r][c] = (fixedBoard[r][c] == 0) ? availableNumbers[idx++] : fixedBoard[r][c];
                }
            }
        }
    }
}

int SudokuSolver::calculateCost() const {
    int cost = 0;
    for (int i = 0; i < N; i++) {
        std::vector<bool> rowCheck(N, false), colCheck(N, false);
        for (int j = 0; j < N; j++) {
            rowCheck[board[i][j] - 1] = true;
            colCheck[board[j][i] - 1] = true;
        }
        for (int k = 0; k < N; k++) {
            if (!rowCheck[k]) cost++;
            if (!colCheck[k]) cost++;
        }
    }
    return cost;
}

std::vector<int> SudokuSolver::swapInBlock(int blockRow, int blockCol) {
    std::uniform_int_distribution<int> dist(0, 2);
    int r1, c1, r2, c2;

    do {
        r1 = blockRow * 3 + dist(generator);
        c1 = blockCol * 3 + dist(generator);
    } while (fixedBoard[r1][c1] != 0);

    do {
        r2 = blockRow * 3 + dist(generator);
        c2 = blockCol * 3 + dist(generator);
    } while (fixedBoard[r2][c2] != 0 || (r1 == r2 && c1 == c2));

    std::swap(board[r1][c1], board[r2][c2]);
    return {r1, c1, r2, c2};
}

int SudokuSolver::computeMarkovChainLength() const {
    int nonFixedCells = 0;
    for (const auto& row : fixedBoard) {
        for (int cell : row) {
            if (cell == 0) nonFixedCells++;
        }
    }
    return nonFixedCells * nonFixedCells;
}

double SudokuSolver::computeInitialTemperature(int sampleSize) {
    std::vector<int> costDifferences;
    int currentCost = calculateCost();
    for (int i = 0; i < sampleSize; ++i) {
        int blockRow = generator() % 3;
        int blockCol = generator() % 3;
        std::vector<int> swaps = swapInBlock(blockRow, blockCol);
        int newCost = calculateCost();
        costDifferences.push_back(newCost - currentCost);
        std::swap(board[swaps[0]][swaps[1]], board[swaps[2]][swaps[3]]);
    }

    double mean = std::accumulate(costDifferences.begin(), costDifferences.end(), 0.0) / sampleSize;
    double variance = 0.0;
    for (int diff : costDifferences) {
        variance += (diff - mean) * (diff - mean);
    }
    variance /= sampleSize;
    return std::sqrt(variance);
}

void SudokuSolver::simulatedAnnealing(double initialTemp, double alpha) {
    double T = initialTemp;
    int currentCost = calculateCost();
    int ml = computeMarkovChainLength();
    std::vector<std::vector<int>> bestBoard = board;
    int bestCost = currentCost;

    int iteration = 0;
    while (bestCost != 0) {
        for (int chain = 0; chain < ml; ++chain) {
            int blockRow = generator() % 3;
            int blockCol = generator() % 3;
            std::vector<int> swaps = swapInBlock(blockRow, blockCol);
            int newCost = calculateCost();
            int deltaCost = newCost - currentCost;

            if (deltaCost < 0 || std::exp(-deltaCost / T) > std::uniform_real_distribution<>(0, 1)(generator)) {
                currentCost = newCost;
                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    bestBoard = board;
                }
            } else {
                std::swap(board[swaps[0]][swaps[1]], board[swaps[2]][swaps[3]]);
            }

            if (bestCost == 0) break;
        }

        T *= alpha;

        if (++iteration % 100 == 0) {
            std::cout << "Iteration " << iteration << ", Cost: " << bestCost << '\n';
        }

        if (iteration >= 1e6) {
            initializeBoard();
            T = initialTemp;
            currentCost = calculateCost();
            bestCost = currentCost;
            iteration = 0;
        }
    }

    board = bestBoard;
    std::cout << "Solved in " << iteration << " iterations!\n";
}

void SudokuSolver::printBoard() const {
    for (const auto& row : board) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << '\n';
    }
}

