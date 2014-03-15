#ifndef Sodoku_H
#define Sodoku_H

#include "Square_Matrix.h"

class sodoku
{
	public:
		// Constructor for the Soduku object.
		sodoku(void);

		// DeConstructor for the Soduku object.
		~sodoku(void);

		// Throws out the current soduku puzzle's contents.
		void display(void);

		// Displays the size of the Sodoku puzzle
		int Get_Size(void);

		// Gets a cell from the puzzle using x and y coordinates.
		// If out of bounds, returns a 0.
		int Get_Cell(const int&, const int&);

		// Sets a cell at x and y coordinates to some value. If out of bounds, returns false.
		bool Set_Cell(const int& x_coordinate, 
					  const int& y_coordinate, 
					  const int& new_value);
	private:
		Square_Matrix matrix;
};

#endif
