#include "Sodoku.h"
#include "Square_Matrix.h"
#include <time.h> // Solely for seeding the RNG.
#include <stdlib.h> // Solely for the RNG.
#include <iostream> // Cout capability.
#include <string> // Couting strings.
#include <thread> // Threads.
#include <chrono> // Used for making threads sleep

using namespace std;

// Constructor for the sodoku object.
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

// Overloaded constructor for the Soduku object with a configurable size. If a
// negative size is given, the size will be set to 0.
Sodoku::Sodoku(int size){
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

// Copies the contents of the other sodoku into this sodoku. Very similar to 
// a copy constructor.
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

// Wipe the contents of the puzzle. This is used when resetting the
// puzzle. Everything but the size of the puzzle is reset.
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

int Sodoku::get_size(void){
	return this->matrix.Get_Size();
}

// Throws out the current sodoku puzzle contents by going row through row, and
// adding spacing between each cells contents for making it easier to read.
// Takes an input string to display on top of the puzzle, and if no message was
// given then don't display a message.
void Sodoku::display(std::string input_string){
	// Will hold the current row.
	vector<int> row_contents;

	// If no input string was given then don't the string.
	if (!input_string.empty())
		cout << input_string << endl;

	// Sets how much padding is needed for each cell display so the columns
	// align properly.
	int padding = 1; 

	// Since we are not expecting values higher than two digits, just manually
	// check if the size is bigger than 1 digit.
	if (this->matrix.Get_Size() > 10)
		padding = 2; // Since we need two spaces for numbers over nine.

	// Go through each row.
	for (int row = 0; row < this->matrix.Get_Size(); ++row)
	{
		// Copies the row into a vector.
		row_contents = this->matrix.Get_Row(row);

		// Makes some nice formatting to the start of the row.
		cout << "| ";

		// Displays the contents of each cell of the row with spacing between cells.
		for (int cell_count = 0; cell_count < this->matrix.Get_Size(); ++cell_count)
		{
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

// Returns the contents of the cell at the X and Y coordinates. If out of bounds,
// returns a 0, of which the behavior is implemented in the Square_Matrix lib.
int Sodoku::get_cell(const int &x, const int &y){
	return this->matrix.Get_Elem(x, y);
}

// Sets a cell at x and y coordinates to some value. If out of bounds, returns false.
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

// Checks if the row is valid, meaning if there are multiples if a number.
// This goes through each element in the row and checks if that element exists 
// elsewhere in the row.
bool Sodoku::check_row_validity(const int& row){
	// Go through each row.
	for (int i = 0; i < this->matrix.Get_Size(); ++i){
		// Check if more than 1 of that int exists in the row.
		for (int j = 0; j < this->matrix.Get_Size(); ++j){
			// If the number appears twice and it is not an unset cell and it is
			// not checking itself, then that row is invalid.
			if ((this->matrix.Get_Elem(i, row) == this->matrix.Get_Elem(j, row)) && (this->matrix.Get_Elem(i, row) != -1) && (i != j))
				return false;
		}
	}

	// If we get here, it means that the row is valid.
	return true;
}

// Checks if column is valid, meaning if there are multiples if a number.
// This goes through each element in the column and checks if that element exists 
// elsewhere in the column.
bool Sodoku::check_column_validity(const int& column){
	// Go through each column.
	for (int i = 0; i < this->matrix.Get_Size(); ++i){
		// Check if more than 1 of that int exists in the column.
		for (int j = 0; j < this->matrix.Get_Size(); ++j){
			// If the number appears twice and it is not an unset cell and it is
			// not checking itself, then that column is invalid.
			if ((this->matrix.Get_Elem(column, i) == this->matrix.Get_Elem(column, j)) && (this->matrix.Get_Elem(column, i) != -1) && (i != j))
				return false;
		}
	}

	// If we get here, it means that the column is valid.
	return true;
}

// Checks if the entire Sodoku puzzle is correct, combines checking each row
// and column into just one function call. This ignores cells which are unset.
bool Sodoku::check_sodoku_validity(void){

	// Since this is a square, we can check rows and columns in one for loop.
	for (int i = 0; i < this->matrix.Get_Size(); ++i)
	{
		if ( this->check_row_validity(i) != true )
			return false;

		if ( this->check_column_validity(i) != true )
			return false;
	}

	// If we get here, it means that the puzzle is valid.
	return true;
}

// Fill in a a percentage of cells with at least one filled. Also adds the cells
// coordinates into a vector which is used to differentiate between nodes which
// can and can't be changed when attempting to solve the puzzle.
//
// This is a heavily modified version of solve_puzzle with the addition of 
// adding the coordinates to const_cells vector, randomly choosing the cells 
// to fill in, and randomly choosing the data to fill it with.
//
// No backtracking or recursion though, as that would be difficult considering
// I need to non serially fill the data which would require a list of attempted
// cells with attempted contents to prevent multiple redundant attempts.
//
// This means that you shouldn't be using this for larger percentages since
// this is a randomized brute force method which is extremely ineficiant. It
// will eventually probably find a puzzle with such a high percentage, but don't
// count on it happening while you still remember you even set it.
//
// It was chosen to be done this way for simplicities sake, as explained
// in the previous paragraph.
//
// NOTE: 
//		THIS DOES NOT GUARANTEE TO GIVE US A SOLVABLE PUZZLE!!
void Sodoku::solve_puzzle_partially(const float& percentage){
	// First we need to find out how many cells to fill.
	int total_cell_count = this->matrix.Get_Size() * this->matrix.Get_Size();
	int trying_to_fill = total_cell_count * percentage;

	// We need to make sure there is at least one partially filled cell for
	// really small puzzles.
	if ( trying_to_fill < 1 ) { trying_to_fill = 1; }

	// While the filled cells is less than the amount of cells we are trying to
	// fill, keep this loop going.
	while (count_filled_cells() != trying_to_fill)
	{
		// Kept for debugging.
		/*cout << std::to_string(this->count_filled_cells()) + " out of " 
			+ std::to_string(this->matrix.Get_Size() * this->matrix.Get_Size()) 
			+ " are filled.\n";*/

		// Make a random set of coordinates and a random int.
		int var = rand() % (this->matrix.Get_Size() - 1);
		int x_coordinate = rand() % (this->matrix.Get_Size());
		int y_coordinate = rand() % (this->matrix.Get_Size());

		// Kept for debugging.
		// cout << std::to_string(x_coordinate) + " " + std::to_string(y_coordinate) + "\n";

		// Write the cell.
		this->set_cell(x_coordinate, y_coordinate, var);

		// Check if the changes are valid, if not then reverse the changes,
		// if they are good then add them into the const_cells entry.
		if ( this->check_sodoku_validity() )
			this->writable.Set_Elem(0, x_coordinate, y_coordinate);
		// If the new cell is not valid, reset that cell back to its original
		// state.
		else
			this->set_cell(x_coordinate, y_coordinate, -1);
	}

	// Reset write attempts per cell since we don't want to count the pre-fill.
	this->heatmap.fill(0);
}

// Fill in a selected amount of cells with at least one filled. Also adds the cells
// coordinates into a vector which is used to differentiate between nodes which
// can and can't be changed when attempting to solve the puzzle.
//
// This is a heavily modified version of solve_puzzle with the addition of 
// adding the coordinates to const_cells vector, randomly choosing the cells 
// to fill in, and randomly choosing the data to fill it with.
//
// Don't use this for a large portion of the array, since it randomly
// tries to solve it instead of backtracking or any other smarter way.
void Sodoku::solve_puzzle_partially_count(int trying_to_fill){
	trying_to_fill = trying_to_fill + this->count_filled_cells();

	// While the filled cells is less than the amount of cells we are trying to
	// fill, keep this loop going.
	while (count_filled_cells() <= trying_to_fill)
	{
		// Kept for debugging.
		/*cout << std::to_string(this->count_filled_cells()) + " out of "
		+ std::to_string(this->matrix.Get_Size() * this->matrix.Get_Size())
		+ " are filled.\n";*/

		// Make a random set of coordinates and a random int.
		int var = rand() % (this->matrix.Get_Size() - 1);
		int x_coordinate = rand() % (this->matrix.Get_Size());
		int y_coordinate = rand() % (this->matrix.Get_Size());

		// Only use those coordinates if they were not set earlier.
		if (!can_set(x_coordinate, y_coordinate))
			continue;

		// Write the cell.
		this->set_cell(x_coordinate, y_coordinate, var);

		// Check if the changes are valid, if not then reverse the changes,
		// if they are good then add them into the const_cells entry.
		if (this->check_sodoku_validity())
			this->writable.Set_Elem(0, x_coordinate, y_coordinate);
		// If the new cell is not valid, reset that cell back to its original
		// state.
		else
			this->set_cell(x_coordinate, y_coordinate, -1);
	}

	// Reset write attempts per cell since we don't want to count the pre-fill.
	this->heatmap.fill(0);
}

// This is the main function for solving the sodoku puzzle.
// 1) Sets the current cell we are trying to solve to the origin cell, 0,0.
// 2) Solves the first cell if it is not unset.
// 3) Goes to the next writable cell and tries to fill it with a valid entry.
// 4) Repeat step 3 until either we reach the last cell and solve it. 
//		If we return to the first writable cell and still can't find a solution 
//		then consider the puzzle unsolvable with the current unwritable cells.
bool Sodoku::solve_puzzle(void){
	// Set the working_cell to -1,-1 when starting to solve the puzzle. Surprisingly,
	// I didn't need to do this in visual studio since it started at 0,0 but in
	// GCC it started at 4410251, 0 which is cool. I think this is because in VS
	// I test everything using a debug mode instead of relase mode, with debug mode
	// forcing all unitialized variables to zero instead of undefined.
	this->working_cell.x = -1;
	this->working_cell.y = -1;

	// Just for when the first cell is not writable. Also checks if there are any
	// writable cells in the first place.
	if (!this->next_cell())
		return false;

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
	//  based on how fast we want either a solution or the puzzle being 
	// marked as unsolvable at the cost of missing a possible solution.
	const unsigned int magic_speed_number = 100000; // 100,000

	// How many steps we should wait till we give the thread a puzzle with a new hint.
	const unsigned long max_steps = (this->get_size() * this->get_size() - this->get_prefilled_cell_count()) * magic_speed_number;
	
	// Check the status of the thread and react accordingly if the
	// thread found a solution or found an unsolvable solution.
	while (true){
		// If the first thread failed, stop all the other threads and indicate
		// failure.
		if (sodokus_for_threads[0].failed_solve){
			this->shutdown_solving_threads(solving_threads, sodokus_for_threads);
			return false;
		}

		// Also check if the first thread found a solution.
		if (sodokus_for_threads[0].is_complete()){
			this->copy(sodokus_for_threads[0]);
			this->shutdown_solving_threads(solving_threads, sodokus_for_threads);
			return true;
		}

		// Loop through all the threads except the initial one and check if
		// and threads found a solution, give new work to threads which failed
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
				solving_threads[i] = std::thread(&Sodoku::solver_for_thread, &sodokus_for_threads[i]);
			}

			// Gives threads which have been going a long time a new sodoku
			// with a new hint. My method of solving a sodoku reacts very well
			// to good hints.
			if (sodokus_for_threads[i].get_amount_of_steps() > max_steps) {
				cout << "Thread " << i << " took too many steps, giving new work." << endl;

				// Get the thread back.
				sodokus_for_threads[i].stop_solving = true;
				solving_threads[i].join();

				// Generate a sodoku with a new hint.
				sodokus_for_threads[i] = *this;
				sodokus_for_threads[i].solve_puzzle_partially_count(1);

				// And replace the old thread with a new thread using the
				// new sodoku.
				solving_threads[i] = std::thread(&Sodoku::solver_for_thread, &sodokus_for_threads[i]);
			}
		}
			
		// Wait a bit so we don't spam checks to the thread, wasting
		// performance.
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Sodoku::solver_for_thread(void){
	// While the puzzle is not solved, keep tring to solve it. stop_solving
	// is for multithreaded purposes, a way to request the thread to stop.
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
				failed_solve = true;
				break;
			}
		}
		else{
			// If going back a a cell failed, then it means we can't go further
			// back, so the puzzle is unsolvable.
			if (!this->back_cell()){
				failed_solve = true;
				break;
			}
		}

		// Uncomment this to see how the puzzle tries to get solved over time.
		// this->display();
		// this->display_heatmap();
		// std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// Tries to fill the specified cell with valid data. Does not check if the
// cell is already valid or not.
bool Sodoku::try_to_fill(const int& x, const int& y){
	// While the cell is not at its maximum value and therefore
	// not all possible values have been tried, keep trying higher values.
	while (this->increment_cell_contents(x, y))
	{
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

// Increments what the current cell we are trying to fill by changing working_cell
// within the sodoku object to the next writable and valid one.
// Starts on the top left of the puzzle to the top right, then down a row from
// left to right, and repeats to the end.
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

// Decrements what the current cell we are trying to fill by changing working_cell
// within the sodoku object to the closest writable and valid one behind this one.
bool Sodoku::back_cell(void)
{
	if ((this->get_cell(2, 8) == 6) && (this->get_cell(6, 7) == 0) && (this->get_cell(8, 6) == 5))
	{
		// This is really useful for debugging, so leave this in.
		//this->display("Went back at [" + std::to_string(this->working_cell.x) + ", " + std::to_string(this->working_cell.y) + "] \n");
	}

	// If currently on the last element of a row, but not the last element
	// of the puzzle, go up one row and to the last column.
	if (this->working_cell.x == 0 && this->working_cell.y != 0)
	{
		// Go to the last node of the above row..
		this->working_cell.x = this->matrix.Get_Size() - 1; 
		this->working_cell.y--; // Go to the next row.
	}
	// If currently on the first element of a row and in the first row, then
	// the node is the first node in the puzzle. Return a false because there
	// are no other nodes to go to.
	else if (this->working_cell.x == 0 && this->working_cell.y == 0)
		return false;

	// If the not first element in the puzzle or row, just decrement the
	// cell coordinates to one cell to the left.
	else
		this->working_cell.x--;

	// If the next cell is one of the const_cells, ones which we are not
	// allowed to modify because they were filled using partial fill, then
	// call back_cell again so we can skip over the const_cell.
	if (this->can_set(this->working_cell.x, this->working_cell.y))
		return true; // If we got here, the node coordinates were succesfully
					 // incremented, so return a true.
	else
		this->back_cell(); // If the next cell was non writable, recursevly
						   // call this function to skip the cell.
}

// Increments the cell by one. This is quicker than having to get the cell
// and then set the cell using two LOC, or three if you want it to look nice.
// Returns a zero if the new value will be out of bounds or if you can't write
// to that cell because it is a constant.
bool Sodoku::increment_cell_contents(const int& column, const int& row){
	// Check if we can write to the cell.
	if ( !this->can_set(column, row) )
		return false;

	// Holds the contents of the cell.
	int cell_contents = this->matrix.Get_Elem(column, row);

	// Increment the contents by one.
	cell_contents++;

	// Check if the new value will be larger than allowed in a sodoku puzzle.
	if ((this->matrix.Get_Elem(column, row) >= this->matrix.Get_Size() - 1))
		return false;

	// Save the new contents into the cell.
	this->set_cell(column, row, cell_contents);

	// We were able to save the new cell contents, so all went well!
	return true;
}

// Checks if the sodoku puzzle is complete, meaning if it is valid and every
// cell is filled. This does not ignore cells which are unset.
bool Sodoku::is_complete(void){
	// First we see if every cell is filled.
	if ( this->count_filled_cells() != (this->matrix.Get_Size() * this->matrix.Get_Size()) )
		return false;

	// Secondly, if the contents of the puzzle are valid.
	if ( !this->check_sodoku_validity() )
		return false;

	// If we get here, it means the sodoku puzzle is complete.
	return true;
}

// Counts how many cells have been filled already.
int Sodoku::count_filled_cells(void){
	// Holds the counter for how many cells have been set.
	int filled_cell_count = 0;

	// Go through each column.
	for (int i = 0; i < this->matrix.Get_Size(); ++i)
	{
		// Check if more than 1 of that int exists in the column.
		for (int j = 0; j < this->matrix.Get_Size(); ++j)
		{
			// If the number appears twice and it is not an unset cell and it is
			// not checking itself, then that column is invalid.
			if (this->matrix.Get_Elem(i, j) >= 0)
				filled_cell_count++;
		}
	}

	return filled_cell_count;
}

// Checks if the input coordinates exist in the vector of coordinates for cells 
// which we are not allowed to modify. Also, if out of bounds return false.
bool Sodoku::can_set(int x_coordinate, int y_coordinate){
	// First check if the coordinates are out of bounds.
	if ((x_coordinate < 0) || (y_coordinate < 0) ||
		(x_coordinate >= this->get_size()) || 
		(y_coordinate >= this->get_size()) )
	{
		return false;
	}

	// Checks if the cell was filled using partial_fill.
	if (this->writable.Get_Elem(x_coordinate, y_coordinate) == 0)
	{
		// Kept for debugging.
		// cout << "Coordinate " + std::to_string(x_coordinate) + ", " + std::to_string(y_coordinate) + "is non_writable\n";
		return false;
	}

	return true;
}

// Returns an idea of how "hard" it was to find the solution.
int Sodoku::get_amount_of_steps(void)
{
	int temp = 0;

	for (int column = 0; column < this->matrix.Get_Size(); column++)
		for (int row = 0; row < this->matrix.Get_Size(); row++)
			temp = temp + this->heatmap.Get_Elem(column, row);

	return temp;
}

// Transfers all the contents of the other sodoku puzzle into
// this one, including the current heatmap, matrix, steps, and
// writable cells.
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

// THIS IS FOR UNIT TESTING ONLY! DON'T USE ME!!
// Not in private because then it couldn't be accessed by the unit tests.
void Sodoku::set_const_cell( int x, int y, int value){
	this->matrix.Set_Elem(value, x, y);

	// Indicate that this cell is not writable.
	this->writable.Set_Elem(0, x, y);
}


// THIS IS FOR UNIT TESTING ONLY! DON'T USE ME!!
// Not in private because then it couldn't be accessed by the unit tests.
void Sodoku::display_heatmap(string input_string){
	// Will hold the current row.
	vector<int> row_contents;

	// If no input string was given then don't the string.
	if (!input_string.empty())
		cout << input_string << endl;

	// Sets how much padding is needed for each cell display so the columns
	// align properly. Uses the largest digit wise heatmap value found.
	//-----------------------------
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
	for (int row = 0; row < this->heatmap.Get_Size(); ++row)
	{
		// Copies the row into a vector.
		row_contents = this->heatmap.Get_Row(row);

		// Makes some nice formatting to the start of the row.
		cout << "| ";

		// Displays the contents of each cell of the row with spacing between cells.
		for (int cell_count = 0; cell_count < this->heatmap.Get_Size(); ++cell_count)
		{
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

// THIS IS FOR UNIT TESTING ONLY! DON'T USE ME!!
// Not in private because then it couldn't be accessed by the unit tests.
void Sodoku::display_writable(string input_string){
	// Will hold the current row.
	vector<int> row_contents;

	// If no input string was given then don't the string.
	if (!input_string.empty())
		cout << input_string << endl;

	// Sets how much padding is needed for each cell display so the columns
	// align properly.
	int padding = 1;

	// Go through each row.
	for (int row = 0; row < this->writable.Get_Size(); ++row)
	{
		// Copies the row into a vector.
		row_contents = this->writable.Get_Row(row);

		// Makes some nice formatting to the start of the row.
		cout << "| ";

		// Displays the contents of each cell of the row with spacing between cells.
		for (int cell_count = 0; cell_count < this->writable.Get_Size(); ++cell_count)
		{
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

// Shut down all the current solving threads. This is a blocking function!
void Sodoku::shutdown_solving_threads(vector<std::thread> &solving_threads, vector<Sodoku> &sodokus_for_threads){
	// The signaling of the thread to shut down and doing .join() are seperated
	// to give the thread time to shut down so we don't waste time waiting.
	for (Sodoku &sodoku : sodokus_for_threads)
		sodoku.stop_solving = true;

	// And now we get them to all join. Blocking!
	for (std::thread &solving_thread : solving_threads)
		solving_thread.join();
}

// Returns how many cells were filled with partial_solve.
int Sodoku::get_prefilled_cell_count(void)
{
	int temp = 0;

	for (int column = 0; column < this->matrix.Get_Size(); column++)
		for (int row = 0; row < this->matrix.Get_Size(); row++)
			if (this->writable.Get_Elem(column, row) == 0)
				temp++;

	return temp;
}
