#include "Sodoku.h"
#include "Square_Matrix.h"
#include <iostream>

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
sodoku::sodoku(void){
	this->matrix.Set_Size(9);
	this->matrix.fill(-1);
}

// DeConstructor for the sodoku object.
sodoku::~sodoku(void){

}

int sodoku::Get_Size(void){
	return this->matrix.Get_Size();
}

// Throws out the current sodoku puzzle contents by going row through row, and
// adding spacing between each cells contents for making it easier to read.
void sodoku::display(void){
	// For saying that these are the contents.
	cout << "Contents of puzzle: " << endl;

	// Will hold the current row.
	vector<int> row_contents;

	// Go through each row.
	for (int row = 0; row < this->matrix.Get_Size(); ++row)
	{
		// Copies the row into a vector.
		row_contents = this->matrix.Get_Row(row);

		// Makes some nice formatting to the start of the row.
		cout << "| ";

		// Displays the contents of each cell of the row with right hand padding.
		for (int cell_count = 0; cell_count < this->matrix.Get_Size(); ++cell_count)
		{
			if (row_contents[cell_count] == -1)
				cout << ". ";
			else
				cout << row_contents[cell_count] << " ";
		}

		// Shows that the row is done.
		cout << "|" << endl;
	}
}

// Returns the contents of the cell at the X and Y coordinates. If out of bounds,
// returns a 0, of which the behavior is implemented in the Square_Matrix lib.
int sodoku::Get_Cell(const int &x, const int &y){
	return this->matrix.Get_Elem(x, y);
}

// Sets a cell at x and y coordinates to some value. If out of bounds, returns false.
bool sodoku::Set_Cell(const int& x, const int& y, const int& val){

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
bool sodoku::check_row_validity(const int& row){
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
}

// Checks if column is valid, meaning if there are multiples if a number.
// This goes through each element in the column and checks if that element exists 
// elsewhere in the column.
bool sodoku::check_column_validity(const int& column){
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
}
