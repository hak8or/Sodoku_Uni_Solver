#include "Sodoku.h"
#include "Square_Matrix.h"
#include <time.h> // Solely for seeding the RNG.
#include <stdlib.h> // Solely for the RNG.
#include <iostream> // Cout capability.
#include <string> // Couting strings.
#include <thread> // Threads.
#include <chrono> // Used for making threads sleep

using namespace std;

/**
 * @brief Creates a puzzle of size 2.
 * 
 * @details Also intitilizes a seed for srand.
 */
Sodoku::Sodoku(void){
	// The actual puzzle itself.
	this->matrix.Set_Size(2);
	this->matrix.fill(-1);

	// How many times each cell was written too.
	this->heatmap.Set_Size(2);
	this->heatmap.fill(0);

	// If the cells are allowed to be changed.
	this->writable.Set_Size(2);
	this->writable.fill(1);

	// Keeps track of how many attempts or tries there were for cells.
	this->amount_of_steps = 0;

	// Flag indicating the given sodoku puzzle is not solvable.
	this->failed_solve = 0;

	// Indicates if the current solving attempt should stop.
	this->stop_solving = 0;

	// Resets the current cell we are working on. While this is set when needed
	// in solve_puzzle, I added it in here for clarities sake.
	working_cell.x = -1;
	working_cell.y = -1;

	// Seed the random number generator using the time.
	srand(time(NULL));
}

/**
 * @brief Creates a puzzle of size size.
 * 
 * @details Also intitilizes a seed for srand.
 * 
 * @param size size of the sodoku puzzle to create.
 */
Sodoku::Sodoku(const int &size){
	// The actual puzzle itself.
	this->matrix.Set_Size(size);
	this->matrix.fill(-1);

	// How many times each cell was written too.
	this->heatmap.Set_Size(size);
	this->heatmap.fill(0);

	// If the cells are allowed to be changed.
	this->writable.Set_Size(size);
	this->writable.fill(1);

	// Keeps track of how many total attempts or tries there were for cells.
	this->amount_of_steps = 0;

	// Resets the current cell we are working on. While this is set when needed
	// in solve_puzzle, I added it in here for clarities sake.
	working_cell.x = -1;
	working_cell.y = -1;

	// Flag indicating the given sodoku puzzle is not solvable.
	this->failed_solve = 0;

	// Indicates if the current solving attempt should stop.
	this->stop_solving = 0;

	// Seed the random number generator using the time.
	srand(time(NULL));
}

/**
 * @brief Overwrites the *this sodoku with the other sodoku.
 * 
 * @details Copies all the attributes of the other sodoku into this once,
 * overwriting all contents of the *this sodoku.
 * 
 * @param other The other sodoku object we will be copying from.
 */
void Sodoku::copy(const Sodoku &other){
	// The actual puzzle itself.
	this->matrix = other.matrix;

	// How many times each cell was written too.
	this->heatmap = other.heatmap;

	// If the cells are allowed to be changed.
	this->writable = other.writable;

	// The current cell that is trying to be filled.
	this->working_cell = other.working_cell;

	// Keeps track of how many total attempts or tries there were for cells.
	this->amount_of_steps = other.amount_of_steps;

	// Flag indicating the given sodoku puzzle is not solvable.
	this->failed_solve = other.failed_solve;

	// Indicates if the current solving attempt should stop.
	this->stop_solving = other.stop_solving;
}

/**
 * @brief Wipe the contents of the puzzle.
 * 
 * @details Resets everything in the puzzle except the size.
 */
void Sodoku::wipe(void){
	// Holds the only object member which is not swapped,
	// the size of the puzzle.
	int size = this->matrix.Get_Size();

	// The actual puzzle itself.
	this->matrix.Set_Size(size);
	this->matrix.fill(-1);

	// How many times each cell was written too.
	this->heatmap.Set_Size(size);
	this->heatmap.fill(0);

	// If the cells are allowed to be changed.
	this->writable.Set_Size(size);
	this->writable.fill(1);

	// Keeps track of how many attempts or tries there were for cells.
	this->amount_of_steps = 0;

	// If the puzzle failed to be solved.
	this->failed_solve = 0;

	// Indicates if the current solving attempt should stop.
	this->stop_solving = 0;

	// Resets the current cell we are working on. While this is set when needed
	// in solve_puzzle, I added it in here for clarities sake.
	working_cell.x = -1;
	working_cell.y = -1;

	// Seed the random number generator using the time.
	srand(time(NULL));
}

/**
 * @brief Returns the size of the puzzle.
 * 
 * @details The size returned is not the total amount of cells, just the length
 * of an edge, so for a matrix of size n x n, the return is n.
 * 
 * @return Size of the edge of the matrix. Not totall amount of cells!
 */
int Sodoku::get_size(void) const{
	return this->matrix.Get_Size();
}

/**
 * @brief Displays the contents of the sodoku puzzle in a graphically friendly way.
 * 
 * @details Uses cout to display the contents of the matrix with appropriate spacing.
 */
void Sodoku::display(void) const{
	// Will hold the current row.
	vector<int> row_contents;

	// Sets how much padding is needed for each cell display so the columns
	// align properly.
	int padding = 1; 

	// Since we are not expecting values higher than two digits, just manually
	// check if the size is bigger than 1 digit.
	if (this->matrix.Get_Size() > 10)
		padding = 2; // Since we need two spaces for numbers over nine.

	// Go through each row.
	for (int row = 0; row < this->matrix.Get_Size(); ++row) {
		// Copies the row into a vector.
		row_contents = this->matrix.Get_Row(row);

		// Makes some nice formatting to the start of the row.
		cout << "| ";

		// Displays the contents of each cell of the row with spacing between cells.
		for (int cell_count = 0; cell_count < this->matrix.Get_Size(); ++cell_count) {
			// Sets up padding for the cell output.
			cout.width(padding);

			if (row_contents[cell_count] == -1)
				cout << "." << " ";
			else
				cout << row_contents[cell_count] << " ";
		}

		// Shows that the row is done.
		cout << " |" << endl;
	}

	// Add in a line to make things look nicer.
	cout << endl;
}

/**
 * @brief Returns the contents of the cell at the X and Y coordinates.
 * @details If coordinates are out of bounds, returns 0.
 * 
 * @param x x position of the cell
 * @param y y position of the cell
 * 
 * @return The contents of the cell.
 */
int Sodoku::get_cell(const int &x, const int &y) const{
	return this->matrix.Get_Elem(x, y);
}

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
bool Sodoku::set_cell(const int& x, const int& y, const int& val){
	// Each write to a cell is considered an attempt or try.
	this->amount_of_steps++;

	// And add that attempt to the heatmap.
	int temp = this->heatmap.Get_Elem(x, y);
	temp++;
	this->heatmap.Set_Elem(temp, x, y);

	// If the coordinates are out of bounds, return false.
	if (x > this->matrix.Get_Size() - 1 || y > this->matrix.Get_Size() - 1)
		return false;

	// Sets the element. Val is first because previous assignment required it to be so.
	this->matrix.Set_Elem(val, x, y);

	// If we got here, then it was in bounds, so return a true.
	return true;
}

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
bool Sodoku::check_row_validity(const int& row) const{
	// Go through each member of the row.
	for (int i = 0; i < this->matrix.Get_Size(); ++i)
		// Check if more than 1 of that member exists in the row.
		for (int j = 0; j < this->matrix.Get_Size(); ++j)
			// If the member appears twice and it is not an unset cell and it is
			// not checking itself, then that row is invalid.
			if ((this->matrix.Get_Elem(i, row) == this->matrix.Get_Elem(j, row)) && (this->matrix.Get_Elem(i, row) != -1) && (i != j))
				return false;

	// If we get here, it means that the row is valid.
	return true;
}

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
bool Sodoku::check_column_validity(const int& column) const{
	// Go through each member of the column.
	for (int i = 0; i < this->matrix.Get_Size(); ++i)
		// Check if more than 1 of that member exists in the column.
		for (int j = 0; j < this->matrix.Get_Size(); ++j)
			// If the member appears twice and it is not an unset cell and it is
			// not checking itself, then that column is invalid.
			if ((this->matrix.Get_Elem(column, i) == this->matrix.Get_Elem(column, j)) && (this->matrix.Get_Elem(column, i) != -1) && (i != j))
				return false;

	// If we get here, it means that the column is valid.
	return true;
}

/**
 * @brief Checks if all the rows and columns of the puzzle follow the provided
 * rules.
 * 
 * @details Ignores any still unfilled variables.
 * 
 * @return True if sodoku follows rules, false if otherwise.
 */
bool Sodoku::check_sodoku_validity(void) const {
	// Since this is a square, we can check rows and columns in one for loop like this.
	for (int i = 0; i < this->matrix.Get_Size(); ++i)
		if ( !this->check_row_validity(i) | !this->check_column_validity(i) )
			return false;

	// If we get here, it means that the puzzle is valid.
	return true;
}

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
void Sodoku::solve_puzzle_partially(const float& percentage){
	// First we need to find out how many cells to fill.
	int total_cell_count = this->matrix.Get_Size() * this->matrix.Get_Size();
	int trying_to_fill = total_cell_count * percentage;

	// Rand does not handle zero's too well, so manually set the contents to zero.
	if (this->get_size() == 1){
		this->set_cell(0, 0, 0);
		this->writable.Set_Elem(0, 0, 0);
		this->heatmap.fill(0);
		return;
	}

	// Make sure to fill at least one partially filled cell for small puzzles.
	if ( trying_to_fill < 1 ) { trying_to_fill = 1; }

	// While the filled cells is less than the amount of cells we are trying to
	// fill, keep this loop going.
	while (count_filled_cells() < trying_to_fill) {
		// Make a random set of coordinates and a random int.
		int var = rand() % (this->matrix.Get_Size() - 1);
		int x_coordinate = rand() % (this->matrix.Get_Size());
		int y_coordinate = rand() % (this->matrix.Get_Size());

		// Write the cell with the new value.
		this->set_cell(x_coordinate, y_coordinate, var);

		// Check if the changes are valid, if not then reverse the changes,
		// if they are good then mark them as non writable.
		if ( this->check_sodoku_validity() )
			this->writable.Set_Elem(0, x_coordinate, y_coordinate);
		else
			this->set_cell(x_coordinate, y_coordinate, -1);
	}

	// Reset write attempts per cell since we don't want to count the pre-fill.
	this->heatmap.fill(0);
}

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
void Sodoku::solve_puzzle_partially_count(int trying_to_fill){
	// How many fills we need to fill by adding the intended additional cells
	// to fill.
	trying_to_fill = trying_to_fill + this->count_filled_cells();

	// Rand does not handle zero's too well, so manually set the contents to zero.
	if (this->get_size() == 1){
		this->set_cell(0, 0, 0);
		this->writable.Set_Elem(0, 0, 0);
		this->heatmap.fill(0);
		return;
	}

	// While the filled cells is less than the amount of cells we are trying to
	// fill, keep this loop going.
	while (count_filled_cells() < trying_to_fill) {
		// Make a random set of coordinates and a random int.
		int var = rand() % (this->matrix.Get_Size() - 1);
		int x_coordinate = rand() % (this->matrix.Get_Size());
		int y_coordinate = rand() % (this->matrix.Get_Size());

		// Only use those coordinates if they were not set earlier.
		if (!can_set(x_coordinate, y_coordinate))
			continue;

		// Write the cell with the new value.
		this->set_cell(x_coordinate, y_coordinate, var);

		// Check if the changes are valid, if not then reverse the changes,
		// if they are good then add them into the const_cells entry.
		if (this->check_sodoku_validity())
			this->writable.Set_Elem(0, x_coordinate, y_coordinate);
		else
			this->set_cell(x_coordinate, y_coordinate, -1);
	}

	// Reset write attempts per cell since we don't want to count the pre-fill.
	this->heatmap.fill(0);
}

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
bool Sodoku::solve_puzzle(void){
	// Make sure we are starting at 1 less than the starting cell. Used for 
	// next_cell to check if the first cell is writable.
	this->working_cell.x = -1;
	this->working_cell.y = -1;

	// If there are no writable cells, return weather the puzzle is solved or not.
	if (!this->next_cell())
		return this->is_complete();

	// Get how many threads we should run based on cores available.
	int threads = thread::hardware_concurrency() - 1;

	// Generate that many different potential sodoku puzzles (with hints).
	vector<Sodoku> sodokus_for_threads;
	for (int i = 0; i < threads; i++){
		sodokus_for_threads.push_back(*this);
		sodokus_for_threads[i].solve_puzzle_partially_count(1);
	}

	// Add another sodoku without any hints to the beginning.
	sodokus_for_threads.insert(sodokus_for_threads.begin(), *this);

	// Generate a thread for each of those puzzles.
	vector<std::thread> solving_threads;
	for (int i = 0; i < sodokus_for_threads.size(); i++)
		solving_threads.push_back(std::thread(&Sodoku::solver_for_thread, &sodokus_for_threads[i]));

	// This keeps track of how many times a thread failed to find a solution. 
	// It sucks to do it this way but I can't easily keep track of what hints
	// I used already. Change this to a better way some day in the future.
	int failed_solves = 0;
	int max_hinted_retries = (this->matrix.Get_Size() * this->matrix.Get_Size()) * 0.75;

	// How many steps of work per cell to solve the puzzle. Change this
	// based on how fast we want either a solution or the puzzle being 
	// marked as unsolvable at the cost of missing a possible solution.
	const unsigned int magic_speed_number = 100000; // 100,000

	// How many steps we should wait till we give the thread a puzzle with a new hint.
	const unsigned long max_steps = 
		(this->get_size() * this->get_size() - this->get_prefilled_cell_count())
		* magic_speed_number;
	
	// Check the status of the thread and react accordingly if the thread found
	// a solution or found an unsolvable solution.
	while (true){
		// If the first thread failed, stop all the other threads and indicate
		// failure.
		if (sodokus_for_threads[0].failed_solve){
			this->shutdown_solving_threads(solving_threads, sodokus_for_threads);
			return false;
		}

		// If the first thread found a solution, copy it to *this and shut down
		// all other threads.
		if (sodokus_for_threads[0].is_complete()){
			this->copy(sodokus_for_threads[0]);
			this->shutdown_solving_threads(solving_threads, sodokus_for_threads);
			return true;
		}

		// Loop through all the threads except the initial one and check if and
		// threads found a solution, give new work to threads which either failed
		// to find a solution after some steps or found out there is no solution.
		for (int i = 1; i < solving_threads.size(); i++){
			// If the thread found a solution, copy the contents of the
			// sodoku from that thread into this one and indicate success.
			if (sodokus_for_threads[i].is_complete()){
				this->copy(sodokus_for_threads[i]);
				this->shutdown_solving_threads(solving_threads, sodokus_for_threads);
				return true;
			}

			// If the thread indicated that there is no solution give it another
			// sodoku with another hint.
			if (sodokus_for_threads[i].failed_solve){
				// Stop generating new threads if we retried more than some amount.
				if (failed_solves >= max_hinted_retries)
					continue; // Skip what is left in the while loop after me.
				else
					failed_solves++;

				// Get the thread back.
				solving_threads[i].join();

				// Generate a sodoku with a new hint.
				sodokus_for_threads[i] = *this;
				sodokus_for_threads[i].solve_puzzle_partially_count(1);

				// And replace the old thread with a new thread using the
				// new sodoku.
				solving_threads[i] = 
					std::thread(&Sodoku::solver_for_thread, &sodokus_for_threads[i]);
			}

			// Gives threads which have been going a long time a new sodoku
			// with a new hint. My method of solving a sodoku reacts very well
			// to good hints.
			if (sodokus_for_threads[i].get_amount_of_steps() > max_steps) {
				cout << "Thread " << i << " took too many steps, giving new work.\n";

				// Get the thread back.
				sodokus_for_threads[i].stop_solving = true;
				solving_threads[i].join();

				// Generate a sodoku with a new hint.
				sodokus_for_threads[i] = *this;
				sodokus_for_threads[i].solve_puzzle_partially_count(1);

				// And replace the old thread with a new thread using the
				// new sodoku.
				solving_threads[i] = 
					std::thread(&Sodoku::solver_for_thread, &sodokus_for_threads[i]);
			}
		}
			
		// Wait a bit so we don't spam checks to the thread, wasting
		// performance.
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


/**
 * @brief Actual sodoku solver running in solving threads.
 * 
 * @details Utilizes backtracking to find a solution for the puzzle.
 */
void Sodoku::solver_for_thread(void){
	// 1) Tries to fill the first avalible cell.
	// 2) Goes to the next writable cell and tries to fill it with a valid entry.
	// 3) If no valid entries were found for the current cell, go back a cell and
	//	  increment the previous one.
	// 4) Repeat step 3 until either we reach the last cell and solve it. 
	//    If we return to the first writable cell and still can't find a solution 
	//    then consider the puzzle unsolvable with the current unwritable cells.

	// While the puzzle is not solved, keep tring to solve it. stop_solving
	// is for multithreaded purposes, a way to request this thread to stop.
	while (!this->failed_solve & !this->stop_solving){
		// If try_to_fill fails, it means that all attempts to fill the current
		// cell have failed, so go back a cell.
		if (this->try_to_fill(this->working_cell.x, this->working_cell.y)){
			// If we detect that the puzzle is solved, exit the loop.
			if (this->is_complete())
				break;

			// If are at the last cell and trying to keep going to the next
			// cell, then the puzzle should be considered unsolvable.
			if (!this->next_cell()){
				this->failed_solve = true;
				break;
			}
		}
		else{
			// If going back a a cell failed, then it means we can't go further
			// back, so the puzzle is unsolvable.
			if (!this->back_cell()){
				this->failed_solve = true;
				break;
			}
		}
	}
}

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
bool Sodoku::try_to_fill(const int& x, const int& y){
	// While the cell is not at its maximum value and therefore
	// not all possible values have been tried, keep trying higher values.
	while (this->increment_cell_contents(x, y)){
		// Check the validity of the changes. 
		if (this->check_column_validity(x) && this->check_row_validity(y))
			return true;
		else
			// If the changes were not valid, keep trying to increment the cell.
			continue;
	}

	// If we got here, then the cell has been incremented all the way
	// to its maximum value but none of the attempted values were correct.
	// Reset the cell contents to an unset state.
	this->set_cell(x, y, -1);

	// Return false to indicate that we can't fill this cell.
	return false;
}

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
bool Sodoku::next_cell(void)
{
	// If currently on the last element of a row, but not the last element
	// of the puzzle, go to the next y and  x = 0
	if (working_cell.x == this->matrix.Get_Size() - 1 &&
		working_cell.y != this->matrix.Get_Size() - 1)
	{
		working_cell.x = 0; // Go to the first node of the row.
		working_cell.y++; // Go to the next row.
	}
	// If currently on the last element of a row and in the last row, then
	// the node is the last node in the puzzle. Return a false because there
	// are no other nodes to go to.
	else if (working_cell.x == this->matrix.Get_Size() - 1 &&
			 working_cell.y == this->matrix.Get_Size() - 1)
		return false;

	// If the not last element in the puzzle or row, just increment the
	// cell coordinates to one cell to the right.
	else if (working_cell.x != this->matrix.Get_Size() - 1)
		working_cell.x++;

	// If the cell was already written to, skip it.
	if (this->get_cell(working_cell.x, working_cell.y) != -1)
		this->next_cell();

	// If the next cell is one of the const_cells, ones which we are not
	// allowed to modify because they were filled using partial fill, then
	// call next_cell again so we can skip over the const_cell.
	if (this->can_set(working_cell.x, working_cell.y))
		return true; // If we got here, the node coordinates were succesfully
					 // incremented, so return a true.
	else
		this->next_cell(); // If the next cell was non writable, recursevly
						   // call this function to skip the cell.
}

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
bool Sodoku::back_cell(void){
	// If currently on the last element of a row, but not the last element
	// of the puzzle, go up one row and to the last column.
	if (this->working_cell.x == 0 && this->working_cell.y != 0) {
		// Go to the last node of the above row.
		this->working_cell.x = this->matrix.Get_Size() - 1; 

		// Go to the above row.
		this->working_cell.y--; 
	}
	// If in the first (top left) cell, return false since we can't go further back.
	else if (this->working_cell.x == 0 && this->working_cell.y == 0)
		return false;

	// If not the first element in the puzzle or row, just decrement the cell
	// coordinates to one cell to the left.
	else
		this->working_cell.x--;

	// If the previous cell is one of the const_cells, ones which we are not
	// allowed to modify because they were filled using partial fill, then
	// call back_cell again so we can skip over the const_cell.
	if (this->can_set(this->working_cell.x, this->working_cell.y))
		return true; // If we got here, the node coordinates were succesfully
					 // incremented, so return a true.
	else
		this->back_cell(); // If the previous cell was non writable, recursevly
						   // call this function to skip the cell.
}

/**
 * @brief Increments the cells contents by one.
 * 
 * @param x X value of cell
 * @param x Y value of cell
 * 
 * @return True if the new value is within bounds, false if otherwise.
 */
bool Sodoku::increment_cell_contents(const int& x, const int& y){
	// Check if the new value will be larger than allowed in a sodoku puzzle.
	if ( this->matrix.Get_Elem(x, y) + 1 >= this->matrix.Get_Size() )
		return false;

	// Save the contents + 1 into the cell.
	this->set_cell(x, y,  this->matrix.Get_Elem(x, y) + 1 );

	// this->matrix.Get_Elem(x, y) is not an int and instead done this
	// way in hopes of it being faster due to not needing to allocate an int.
	// Not sure how much of a boost this gives, if any, and I am still learning
	// how to use a profiler correctly, so this is left like this.

	// We were able to save the new cell contents, so all went well!
	return true;
}

/**
 * @brief Checks if the sodoku puzzle is complete, with all cells filled and the
 * contents following the rules given for a Sodoku puzzle.
 * 
 * @return True if the puzzle is completed, false if otherwise.
 */
bool Sodoku::is_complete(void) const{
	// First we see if every cell is filled.
	if ( this->count_filled_cells() != (this->matrix.Get_Size() * this->matrix.Get_Size()) )
		return false;

	// Secondly, if the contents of the puzzle are valid.
	if ( !this->check_sodoku_validity() )
		return false;

	// If we get here, it means the sodoku puzzle is complete.
	return true;
}

/**
 * @brief Counts how many cells have been filled with some value.
 * 
 * @return How many cells have been filled with some value.
 */
int Sodoku::count_filled_cells(void) const {
	// Holds the counter for how many cells have been set.
	int filled_cell_count = 0;

	// Increments counter for every filled cell.
	for (int x = 0; x < this->matrix.Get_Size(); ++x)
		for (int y = 0; y < this->matrix.Get_Size(); ++y)
			if (this->matrix.Get_Elem(x, y) >= 0)
				filled_cell_count++;

	return filled_cell_count;
}

/**
 * @brief Checks if the cell is writable.
 * 
 * @param x X value of cell
 * @param y Y value of cell
 * 
 * @return True if writable, false if otherwise.
 */
bool inline Sodoku::can_set(const int &x, const int &y){
	return this->writable.Get_Elem(x, y);
}

/**
 * @brief Gets the amount of steps taken, which can be considered how difficult
 * it was, to solve the puzzle.
 * 
 * @return Amount of steps taken to solve.
 */
int Sodoku::get_amount_of_steps(void){
	int steps = 0;

	// Increments counter for every filled cell.
	for (int x = 0; x < this->matrix.Get_Size(); x++)
		for (int y = 0; y < this->matrix.Get_Size(); y++)
			steps = steps + this->heatmap.Get_Elem(x, y);

	return steps;
}

/**
 * @brief Copies the sodoku puzzle into *this one.
 * 
 * @details Copies all the contents of the other sodoku puzzle into this one,
 * including the current heatmap, matrix, steps, and  writable cells.
 * 
 * @param other_matrix The matrix we are copying from.
 * 
 * @return The sodoku.
 */
Sodoku Sodoku::operator&=(const Sodoku other_matrix){
	// Gotta check self assignment first!
	if (this == &other_matrix)
		return *this;

	// While this operator should not require a deep copy and
	// therefore not a custom operator implimentation, I added
	// it in just in case and for clarities sake.
	this->heatmap = other_matrix.heatmap;
	this->writable = other_matrix.writable;
	this->matrix = other_matrix.matrix;
	this->working_cell = other_matrix.working_cell;
	this->amount_of_steps = other_matrix.amount_of_steps;
	this->failed_solve = other_matrix.failed_solve;
	this->working_cell = other_matrix.working_cell;
}

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
void Sodoku::set_const_cell(const int &x, const int &y, const int &value){
	this->matrix.Set_Elem(value, x, y);

	// Indicate that this cell is not writable.
	this->writable.Set_Elem(0, x, y);
}

/**
 * @brief Displays how many steps were taken to solve the puzzle for each cell
 * using cout.
 */
void Sodoku::display_heatmap(void){
	// Will hold the current row.
	vector<int> row_contents;

	// Sets how much padding is needed for each cell display so the columns
	// align properly. Uses the largest digit wise heatmap value found.
	int largest_heatmap_value = 0;
	int padding = 1;

	// Gets the largest heatmap value.
	for (int y = 0; y < this->matrix.Get_Size(); ++y)
		for (int x = 0; x < this->matrix.Get_Size(); ++x)
			if (this->heatmap.Get_Elem(x, y) > largest_heatmap_value)
				largest_heatmap_value = this->heatmap.Get_Elem(x, y);

	// Find out how many digits that value has.
	while ( largest_heatmap_value /= 10 )
		padding++;

	// Go through each row.
	for (int row = 0; row < this->heatmap.Get_Size(); ++row){
		// Copies the row into a vector.
		row_contents = this->heatmap.Get_Row(row);

		// Makes some nice formatting to the start of the row.
		cout << "| ";

		// Displays the contents of each cell of the row with spacing between cells.
		for (int cell_count = 0; cell_count < this->heatmap.Get_Size(); ++cell_count){
			// Sets up padding for the cell output.
			cout.width(padding);

			if (row_contents[cell_count] == -1)
				cout << "." << " ";
			else
				cout << row_contents[cell_count] << " ";
		}

		// Shows that the row is done.
		cout << " |" << endl;
	}

	// Add in a line to make things look nicer.
	cout << endl;
}

/**
 * @brief Signals all threads to shut down and then waits for them to stop.
 * 
 * @details This is a blocking function!
 * 
 * @param sodokus_for_threads A vector of threads and the sodoku each thread is working on.
 */
void Sodoku::shutdown_solving_threads(vector<std::thread> &solving_threads, 
	vector<Sodoku> &sodokus_for_threads){
	// The signaling of the thread to shut down and doing .join() are seperated
	// to give the thread time to shut down so we don't waste time waiting.
	for (Sodoku &sodoku : sodokus_for_threads)
		sodoku.stop_solving = true;

	// And now we get them to all join. Blocking!
	for (std::thread &solving_thread : solving_threads)
		solving_thread.join();
}

/**
 * @brief Counts how many fills were made via partial_fill.
 * 
 * @details Set was generated by partial fill if is not writable.
 * 
 * @return Count of pre filled cells.
 */
int Sodoku::get_prefilled_cell_count(void) const{
	int prefilled_cell_count = 0;

	for (int x = 0; x < this->matrix.Get_Size(); x++)
		for (int y = 0; y < this->matrix.Get_Size(); y++)
			if (this->writable.Get_Elem(x, y) == 0)
				prefilled_cell_count++;

	return prefilled_cell_count;
}
