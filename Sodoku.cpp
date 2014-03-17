#include "Sodoku.h"
#include "Square_Matrix.h"
#include <time.h> // Solely for seeding the RNG.
#include <stdlib.h> // Solely for the RNG.
#include <iostream>
#include <string>

// NOTE:
// Because I do not wish to modify Square_Matrix from the previous assignment, 
// some issues are dealt with in this project. For example, to deal with cells
// existing but not yet written to, I fill the entire matrix with -1's and if a 
// -1 is found it is considered to be not yet written to. I don't know how to
// handle Get_Cell.

// Gotta use a namespace or else all my couts will have to specify the namespace
// each time.
using namespace std;

// Constructor for the sodoku object.
Sodoku::Sodoku(void){
	this->matrix.Set_Size(2);
	this->matrix.fill(-1);
	this->trying_to_fill = 0;

	// Seed the random number generator using the time.
	srand(time(NULL));

	// Enable this to see how the puzzle gets solved step by step.
	this->view_progress = 0;
}

// Overloaded constructor for the Soduku object with a configurable size. If a
// negative size is given, the size will be set to 0.
Sodoku::Sodoku(int size){
	this->matrix.Set_Size(size);
	this->matrix.fill(-1);
	this->trying_to_fill = 0;

	// Seed the random number generator using the time.
	srand(time(NULL));

	// Enable this to see how the puzzle gets solved step by step.
	this->view_progress = 0;
}

// DeConstructor for the sodoku object.
Sodoku::~Sodoku(void){

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
	// Holds the row of int's
	std::vector<int> row_contents = this->matrix.Get_Row(row);

	// Go through each row.
	for (int i = 0; i < row_contents.size(); ++i)
	{
		// Check if more than 1 of that int exists in the row.
		for (int j = 0; j < row_contents.size(); ++j)
		{
			// If the number appears twice and it is not an unset cell and it is
			// not checking itself, then that row is invalid.
			if ((row_contents[i] == row_contents[j]) && (row_contents[i] != -1) && (i != j))
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
	// Holds the column of int's
	std::vector<int> column_contents = this->matrix.Get_Column(column);

	// Go through each column.
	for (int i = 0; i < column_contents.size(); ++i)
	{
		// Check if more than 1 of that int exists in the column.
		for (int j = 0; j < column_contents.size(); ++j)
		{
			// If the number appears twice and it is not an unset cell and it is
			// not checking itself, then that column is invalid.
			if ((column_contents[i] == column_contents[j]) && (column_contents[i] != -1) && (i != j))
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
// 		NOTE
// This ONLY works for small percentages since it can only go back one "node",
// so if it turns out that no matter what you try to the current node and it 
// still won't be valid, then this will fail.
void Sodoku::partial_fill(const float& percentage){
	// First we need to find out how many cells to fill.
	int total_cell_count = this->matrix.Get_Size() * this->matrix.Get_Size();
	this->trying_to_fill = total_cell_count * percentage;

	// We need to make sure there is at least one partially filled cell for
	// really small puzzles.
	if ( this->trying_to_fill < 1 ) { this->trying_to_fill = 1; }

	// Keeps track of how many cells were filled.
	int filled = 0;

	// While the filled cells is less than the amount of cells we are trying to
	// fill, keep this loop going.
	while (filled != this->trying_to_fill)
	{
		// Make a random set of coordinates and a random int.
		int var = rand() % (this->matrix.Get_Size());
		int x_coordinate = rand() % (this->matrix.Get_Size());
		int y_coordinate = rand() % (this->matrix.Get_Size());

		// If this coordinate has not been already set earlier, try to put a
		// new value into it.
		if (this->matrix.Get_Elem(x_coordinate, y_coordinate) == -1)
		{
			// Write the cell.
			this->matrix.Set_Elem(var, x_coordinate, y_coordinate);

			// Check if the changes are valid, if not then reverse the changes,
			// if they are good then add them into the const_cells entry.
			if ( this->check_sodoku_validity() )
			{
				// Add the new coordinates to the list.
				const_cells.push_back( coordinates() );
				const_cells[filled].x = x_coordinate;
				const_cells[filled].y = y_coordinate;

				// Increment the amount of filled cells.
				filled++;
			}
			// If the new cell is not valid, reset that cell back to its original
			// state.
			else
				this->matrix.Set_Elem(-1, x_coordinate, y_coordinate);
		}
	}
}

// This is the main function for solving the sodoku puzzle.
// 1) Sets the current cell we are trying to solve to the origin cell, 0,0.
// 2) Solves the first cell if it is not unset.
// 3) Goes to the next writable cell and tries to fill it with a valid entry.
// 4) Repeat step 3 until either we reach the last cell and solve it. 
//		If we return to the first writable cell and still can't find a solution 
//		then consider the puzzle unsolvable with the current unwritable cells.
bool Sodoku::solve_puzzle(void){
	// Set the working_cell to 0,0 when starting to solve the puzzle. Surprisingly,
	// I didn't need to do this in visual studio since it started at 0,0 but in
	// GCC it started at 4410251, 0 which is cool.
	this->working_cell.x = 0;
	this->working_cell.y = 0;

	// Manually try to solve the first cell if it isn't a const to get started.
	if (can_set(working_cell.x, working_cell.y))
		this->try_to_fill(this->working_cell.x, this->working_cell.y);

	// Starts on the top left of the puzzle to the top right, then down a row 
	// from left to right, and repeats to the end.
	while (this->next_cell()){
		// Stick a display here to see how the matrix is solved step by step.
		// this->display();

		// If try_to_fill fails, it means that all attempts to fill the current
		// cell have failed, so go back a cell.
		if (this->try_to_fill(this->working_cell.x, this->working_cell.y)){
			// Increment the amount of steps it took so far to solve this puzzle.
			amount_of_steps++;
		}
		else{
			// Thrown in here for debugging so output is easier to navigate.
			// cout << "Backtracing now!\n";

			// First we need to reset the current cell to the unset value though.
			this->matrix.Set_Elem(-1, working_cell.x, working_cell.y);

			// This is done twice because in the while loop here both checking
			// and incrementing are done twice. If we only go back once, then
			// after the while conditional both checks and increments we will
			// be right back to the same cell. Chaining these together into
			// one if statement for some reason does not work.
			// If going back a a cell failed, then it means we can't go further
			// back, so the puzzle is unsolvable.
			if (this->back_cell())
			{
				if(!this->back_cell())
					 // Return false to show it was unable to be solved.
					return false;
			}
			else 
				return false;
		}
	}

	// If we were able to get here, then it means the puzzle has been
	// fully solved, so return true.
	return true;
}

// Tries to fill the specified cell with valid data. Does not check if the
// cell is already valid or not.
bool Sodoku::try_to_fill(const int& x, const int& y){
	// Contents of the cell when originally hit this function 
	// which will be used for resetting the variable back to its
	// original state if we can't fill the cell.
	int orig_contents = this->get_cell(x, y);

	// While the cell is not at its maximum value and therefore
	// not all possible values have been tried, keep trying higher values.
	while (this->increment_cell(x, y))
	{
		// Used if the user wants to see how the puzzle gets changed over time.
		if (this->view_progress)
			this->display();

		// Check the validity of the changes. 
		if (check_sodoku_validity())
			return true;
		else
			// If the changes were not valid, keep trying to increment the cell.
			continue;
	}

	// If we got here, then the cell has been incremented all the way
	// to its maximum value but none of the attempted values were correct.
	// Reset the cell contents to what they were when we first tried to 
	// fill the cell.
	this->set_cell(x, y, orig_contents);

	// Used if the user wants to see how the puzzle gets changed over time.
	if (this->view_progress)
			this->display();

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

	// If the next cell is one of the const_cells, ones which we are not
	// allowed to modify because they were filled using partial fill, then
	// call next_cell again so we can skip over the const_cell.
	if (can_set(working_cell.x, working_cell.y))
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
	// call next_cell again so we can skip over the const_cell.
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
bool Sodoku::increment_cell(const int& column, const int& row){
	if ((row < this->matrix.Get_Size()) && // Is the row out of bounds?
		(column < this->matrix.Get_Size()) && // Is the column out of bounds?
		(this->get_cell(column, row) < this->matrix.Get_Size() - 1)) // Is the new
		// value out of bounds?
	{
		// Holds the contents of the cell.
		int cell_contents = this->get_cell(column, row);

		// Increment the contents by one.
		cell_contents++;

		// Save the new contents into the cell.
		this->matrix.Set_Elem(cell_contents, column, row);

		// We were able to save the new cell contents, so all went well!
		return true;
	}
	else
	{
		// If we got here, then something went wrong, so return a false.
		return false;
	}

}

void Sodoku::set_const_cell(int value, int x, int y){
	this->matrix.Set_Elem(value, x, y);

	// Add the new coordinates to the list.
	coordinates const_cell;
	const_cell.x = x;
	const_cell.y = y;
	const_cells.push_back( const_cell );
}

// Decrements the cell by one. This is quicker than having to get the cell
// and then set the cell using two LOC, or three if you want it to look nice.
// Returns a zero if the new value will be out of bounds or if you can't write
// to that cell because it is a constant.
bool Sodoku::decrement_cell(const int& column, const int& row){
	if (row < this->matrix.Get_Size() && // Is the row out of bounds?
		column < this->matrix.Get_Size() && // Is the column out of bounds?
		this->can_set(column, row) && // Is the cell writable?
		this->get_cell(column, row) > -1) // Is the new already too small?
	{
		// Holds the contents of the cell.
		int cell_contents = this->get_cell(column, row);

		// Increment the contents by one.
		cell_contents--;

		// Save the new contents into the cell.
		this->matrix.Set_Elem(cell_contents, column, row);

		// We were able to save the new cell contents, so all went well!
		return true;
	}
	// If we got here, then something went wrong, so return a false.
	else
		return false;
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
			if (this->matrix.Get_Elem(i, j) != -1)
				filled_cell_count++;
		}
	}

	return filled_cell_count;
}

// Checks if the input coordinates exist in the vector of coordinates for cells 
// which we are not allowed to modify.
bool Sodoku::can_set(int x_coordinate, int y_coordinate){
	for (int i = 0; i < this->const_cells.size(); ++i)
		if ((this->const_cells[i].x == x_coordinate) &&
			(this->const_cells[i].y == y_coordinate))
		{
			return false;
		}

	return true;
}
