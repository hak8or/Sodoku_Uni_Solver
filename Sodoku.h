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
	private:
		Square_Matrix matrix;
};

#endif
