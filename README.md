# Sudoku Solver Using Simulated Annealing

This project implements a Sudoku solver using **Simulated Annealing**, a probabilistic optimization algorithm. The solver is designed to fill in a partially completed Sudoku puzzle and find a valid solution that satisfies the Sudoku constraints.

## How It Works

### Simulated Annealing
Simulated Annealing is a probabilistic algorithm inspired by the annealing process in metallurgy. It starts with a high "temperature" and progressively lowers it to minimize a cost function, exploring solutions that may temporarily increase the cost to escape local minima.

### Steps
1. **Initialization**: 
   - Fill the Sudoku board while respecting the fixed numbers.
   - Generate random values for the remaining cells in each 3x3 block.
2. **Cost Function**:
   - Counts the number of duplicates in rows and columns.
   - A cost of `0` means the puzzle is solved.
3. **Annealing**:
   - Randomly swaps two cells within a 3x3 block.
   - Accepts or rejects the swap based on the change in cost and current temperature.
4. **Cooling**:
   - Reduces the temperature at each step based on a cooling rate (`alpha`).
5. **Restart**:
   - If the algorithm gets stuck, it reinitializes the board and restarts.

