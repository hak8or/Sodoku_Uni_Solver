#ifndef Sodoku_H
#define Sodoku_H

#include "Square_Matrix.h"
#include <string> // Couting strings.
#include <thread> // For threads

/**
 * @brief Holds the puzzle and all related stats.
 * 
 * @details The rules state that the puzzle is a square, no number can repeat in
 * a row or column, and that all cells must be between 0 and the size of the puzzle.
 * 
 * Uses a multithreaded solver to find a solution as fast as possible.
 */
class Sodoku {
public:
	/**
	 * @brief Creates a puzzle of size 2.
	 * 
	 * @details Also intitilizes a seed for srand.
	 */
	Sodoku(void);

	/**
	 * @brief Creates a puzzle of size size.
	 * 
	 * @details Also intitilizes a seed for srand.
	 * 
	 * @param size size of the sodoku puzzle to create.
	 */
	Sodoku(int size);

	/**
	 * @brief Wipe the contents of the puzzle.
	 * 
	 * @details Resets everything in the puzzle except the size.
	 */
	void wipe(void);

	/**
	 * @brief Displays the contents of the sodoku puzzle in a graphically friendly way.
	 * It also takes an input string and displays the string on the top of the puzzle.
	 * 
	 * @details Uses cout to display both the contents of the matrix with appropriate
	 * spacing and the string on top.
	 * 
	 * @param input_string String we will display.
	 */
	void display(std::string input_string = "");

	/**
	 * @brief Returns the size of the puzzle.
	 * 
	 * @details The size returned is not the total amount of cells, just the length
	 * of an edge, so for a matrix of size n x n, the return is n.
	 * 
	 * @return Size of the edge of the matrix. Not totall amount of cells!
	 */
	int get_size(void);

	/**
	 * @brief Returns the contents of the cell at the X and Y coordinates.
	 * @details If coordinates are out of bounds, returns 0.
	 * 
	 * @param x x position of the cell
	 * @param y y position of the cell
	 * 
	 * @return The contents of the cell.
	 */
	int get_cell(const int& x_coordinate, const int& y_coordinate);

	/**
	 * @brief Sets a cell at x and y coordinates to some value. 
	 * 
	 * @details If out of bounds, returns false.
	 * 
	 * @param x x position of the cell
	 * @param y y position of the cell
	 * @param val Value written to cell
	 * 
	 * @return True if coordinates were in bounds. False if otherwise.
	 */
	bool set_cell(const int& x_coordinate, 
				  const int& y_coordinate, 
				  const int& new_value);

	/**
	 * @brief Checks if all the rows and columns of the puzzle follow the provided
	 * rules.
	 * 
	 * @details Ignores any still unfilled variables.
	 * 
	 * @return True if sodoku follows rules, false if otherwise.
	 */
	bool check_sodoku_validity(void);

	/**
	 * @brief Solves a percentage of the puzzle with at least 1 cell.
	 * 
	 * @details Randomized fill of the puzzle instead of backtrack, so this is a very
	 * inefficiant way to fill a puzzle. This means that you shouldn't be using this
	 * for larger percentages since this is a randomized brute force method which is
	 * extremely ineficiant. It will eventually probably find a puzzle with such a 
	 * high percentage, but don't count on it happening while you still remember you
	 * even set it.
	 * 
	 * Do not use this for an already partially filled puzzle!
	 * 
	 * NOTE:
	 * 		THIS DOES NOT GUARANTEE TO GIVE US A SOLVABLE PUZZLE!!
	 * 		
	 * @param percentage Percentage of how much to fill, from 0 to 1.0
	 */
	void solve_puzzle_partially(const float&);

	/**
	 * @brief Solves a number of cells of the puzzle.
	 * 
	 * @details Randomized fill of the puzzle instead of backtrack, so this is a very
	 * inefficiant way to fill a puzzle. This means that you shouldn't be using this
	 * for larger portion since this is a randomized brute force method which is
	 * extremely ineficiant. It will eventually probably find a puzzle with a high 
	 * prefill count, but don't count on it happening while you still remember you
	 * even set it.
	 * 
	 * NOTE:
	 * 		THIS DOES NOT GUARANTEE TO GIVE US A SOLVABLE PUZZLE!!
	 * 		
	 * @param trying_to_fill Number of cells to fill.
	 */
	void solve_puzzle_partially_count(int trying_to_fill);

	/**
	 * @brief Checks if the sodoku puzzle is complete, with all cells filled and the
	 * contents following the rules given for a Sodoku puzzle.
	 * 
	 * @return True if the puzzle is completed, false if otherwise.
	 */
	bool is_complete(void);

	/**
	 * @brief Solve the puzzle using multithreading.
	 * 
	 * @details Generates the same number of threads as cores detected, with the first
	 * thread being the initial puzzle, and the remainder having a hint added. Some
	 * puzzles can be solved in a very small number of steps if a good hint is given,
	 * so my implimentation heavily favors situations when more hints can be supplied
	 * via more threads and therefore cores. A max amount of steps for the non initial
	 * thread are intended to exploit that hint capability, so once a thread hits more
	 * than a number of steps, it is given the puzzle but with a new hint.
	 * 
	 * @return True if a solution has been found, False if tried all permutations with
	 * no found solution.
	 */
	bool solve_puzzle(void);

	/**
	 * @brief THIS IS FOR UNIT TESTING ONLY! DON'T USE ME!!
	 * 
	 * @details Not in private because then it couldn't be accessed by the unit tests.
	 * Sets the element to the value and marks that element as non writable. Meant
	 * to emulate partial_solve for unit tests.
	 * 
	 * @param x X value of cell
	 * @param y Y value of cell
	 * @param value value we will write to the cell.
	 */
	void set_const_cell(int x, int y, int value);

	/**
	 * @brief Gets the amount of steps taken, which can be considered how difficult
	 * it was, to solve the puzzle.
	 * 
	 * @return Amount of steps taken to solve.
	 */
	int get_amount_of_steps(void);

	/**
	 * @brief Displays how many steps were taken to solve the puzzle for each cell.
	 */
	void display_heatmap(void);

	/**
	 * @brief Copies the sodoku puzzle into *this one.
	 * 
	 * @details Copies all the contents of the other sodoku puzzle into this one,
	 * including the current heatmap, matrix, steps, and  writable cells.
	 * 
	 * @param other_matrix The matrix we are copying from.
	 * @return The sodoku.
	 */
	Sodoku operator&=(const Sodoku other_matrix);

private:
	/**
	 * @brief Counts how many cells have been filled with some value.
	 * 
	 * @return How many cells have been filled with some value.
	 */
	int count_filled_cells(void);

	/**
	 * @brief Goes to the next writable cell.
	 * 
	 * @details 
	 * Path taken:
	 * 			-----\
	 * 			/----/
	 * 			\-----
	 * 			
	 * @return True if we were able to go forwards to a writable cell. False if unable
	 * to do so, signaling there are no writable cells ahead the current one.
	 */
	bool next_cell(void);


	/**
	 * @brief Tries to go back to a cell that is writable.
	 * 
	 * @details 
	 * Path taken:
	 * 			-----\
	 * 			/----/
	 * 			\-----
	 * 			
	 * @return True if we were able to go back to a writable cell. False if unable
	 * to do so, signaling there are no writable cells behind the current one.
	 */
	bool back_cell(void);

	/**
	 * @brief Increments the cells contents by one.
	 * 
	 * @param column X value of cell
	 * @param row Y value of cell
	 * 
	 * @return True if the new value is within bounds and the cell is writable, false
	 * if otherwise.
	 */
	bool increment_cell_contents(const int& column, const int& row);

	/**
	 * @brief Checks if the cell is writable.
	 * 
	 * @param x_coordinate X value of cell
	 * @param y_coordinate Y value of cell
	 * 
	 * @return True if writable, false if otherwise.
	 */
	bool inline can_set(int x, int y);

	/**
	 * @brief Tries to fill the cell with a value that follows sodoku rules.
	 * @details Increments the cell from its current value to a valid value.
	 * 
	 * @param x X value of cell
	 * @param y Y value of cell
	 * 
	 * @return True if was able to find a next cell that is valid, false if there
	 * are no more possible values larger than the current cell which are valid.
	 */
	bool try_to_fill(const int &x, const int &y);

	/**
	 * @brief Checks if the row is valid as per the given sodoku rules.
	 * 
	 * @details Checks if there is a repeat of any number except the initial "unset"
	 * number.
	 * 
	 * @param row What row to check.
	 * 
	 * @return True if row is valid, false if row is not valid.
	 */
	bool check_row_validity(const int& row);

	/**
	 * @brief Checks if the column is valid as per the given sodoku rules.
	 * 
	 * @details Checks if there is a repeat of any number except the initial "unset"
	 * number.
	 * 
	 * @param column What column to check.
	 * 
	 * @return True if column is valid, false if column is not valid.
	 */
	bool check_column_validity(const int& column);

	/**
	 * @brief Actual sodoku solver running in solving threads.
	 * 
	 * @details Utilizes backtracking to find a solution for the puzzle.
	 */
	void solver_for_thread(void);

	/**
	 * @brief Overwrites the *this sodoku with the other sodoku.
	 * 
	 * @details Copies all the attributes of the other sodoku into this once,
	 * overwriting all contents of the *this sodoku.
	 * 
	 * @param other The other sodoku object we will be copying from.
	 */
	void copy(const Sodoku &other);

	/**
	 * @brief Counts how many fills were made via partial_fill.
	 * 
	 * @details Set was generated by partial fill if is not writable.
	 * 
	 * @return Count of pre filled cells.
	 */
	int get_prefilled_cell_count(void);

	/**
	 * @brief Signals all threads to shut down and then waits for them to stop.
	 * 
	 * @details This is a blocking function!
	 * 
	 * @param sodokus_for_threads A vector of threads and the sodoku each thread is working on.
	 */
	void shutdown_solving_threads( std::vector<std::thread> &solving_threads,
		std::vector<Sodoku> &sodokus_for_threads);

	// Holds the contents of the various cells for the puzzle.
	Square_Matrix matrix;

	// Coordinates of a cell which is not allowed to change. Meant for holding
	// cells which were partially filled when the puzzle is first presented.
	struct coordinates { int x; int y; };

	// The coordinates of the current cell we are trying to fill.
	coordinates working_cell;

	// Keeps track of how many attempts or tries there were for cells.
	// Each write counts as an attempt or try. While not ideal for
	// measuring how difficult it was, it gives a good idea.
	int amount_of_steps;

	// Heatmap for attempts per cell.
	Square_Matrix heatmap;

	// Map saying if we can or can't write to the cell (partial_fill'ed).
	Square_Matrix writable;

	// Flag indicating the given sodoku puzzle is not solvable.
	bool failed_solve;

	// Indicates if the current solving attempt should stop.
	// Only for controling other threads. This is my first fooray
	// into threads and there are abundant bad design choices here, with this
	// being one of them!
	bool stop_solving;

	// Factorial function which might be used to calculate max theoretical
	// attempts to solve a sodoku puzzle.
	inline int factorial(int n) { return ((n == 1 || n == 0) ? 1 : factorial(n - 1) * n); }
};

#endif
