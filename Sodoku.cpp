#include "Sodoku.h"
#include "Square_Matrix.h"
#include <iostream>

// Gotta use a namespace or else all my couts will have to specify the namespace
// each time.
using namespace std;

// Constructor for the sodoku object.
sodoku::sodoku(void){
	this->matrix.Set_Size(9);
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
			cout << row_contents[cell_count] << " ";

		// Shows that the row is done.
		cout << "|" << endl;
	}
}

// Returns the contents of the cell at the X and Y coordinates. If out of bounds,
// returns a 0, of which the behavior is implemented in the Square_Matrix lib.
int sodoku::Get_Cell(const int &x, const int &y){
	return this->matrix.Get_Elem(x, y);
}
