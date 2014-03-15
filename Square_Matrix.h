/* 	
	Author: Marcin Ziemianowicz
	Class: CSCI 235 (Descrete Analysis 2) Section 01
	Professor: Pavel Shostak
*/
	
#ifndef Square_Matrix_H
#define Square_Matrix_H

#include <vector>

// The Square_Matrix class as per the assignment.
// Object of this class stores integer matrix of size NxN. This matrix is 
// represented as dynamically allocated two-dimentional array of integers.
class Square_Matrix
{
	public:
		// Constructor
		Square_Matrix(void);

		// Destructor
		~Square_Matrix(void);

		// Allocates memory for the square matrix of given size (new_size) and 
		// sets the size of matrix to it. If the object already stored matrix 
		// all the values in it are lost.
		// Avoid memory leaks! If memory for the matrix was allocated before, 
		// make sure your properly release it before allocating more memory for 
		// new matrix size! 
		void Set_Size (const int &new_size);

		// Method returns current size of the matrix.
		int Get_Size(void);

		// Sets element on the intersection of given row (num_row) and given 
		// column (num_column) to new value (new_value).
		void Set_Elem (const int &new_value, const int &num_column, const int &num_row);

		// Returns the value of the element on the intersection of given 
		// row (num_row) and given column (num_column).
		int Get_Elem (const int &num_column, const int &num_row);

		// Returns the selected Row in the form of a vector.
		std::vector<int> Get_Row(const int &num_row);

		// Returns the selected column in the form of a vector.
		std::vector<int> Get_Column(const int &num_column);

		// Fills the entire matrix with this int.
		void fill(const int &value);

		// I am here just to see if all is working.
		// Should return 2014.
		int sanity_check(void);

		// Equality overloader, checks if size and contents are the same.
		bool operator==(const Square_Matrix &other_matrix);

		// Addition overloader
		Square_Matrix &operator+(const Square_Matrix &other_matrix);

		// Assignment operator, assume I don't need to worry about self assignment.
		// A class usually sets this up automatically, but it is a good
		// idea to impliment it yourself to make sure all is well since
		// when default it might break.
		Square_Matrix &operator=(const Square_Matrix &other_matrix);

		// Copy Constructor
		Square_Matrix(const Square_Matrix &other_matrix);

	private:
		// Size of the matrix, assuming matrix is a square.
		int size;

		// Pointer to pointer for matrix contents (rows, columns).
		int **matrix;
};
#endif
