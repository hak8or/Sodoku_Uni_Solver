/*
Author: Marcin Ziemianowicz
Class: CSCI 235 (Descrete Analysis 2) Section 01
Professor: Pavel Shostak
*/

#include <iostream>
#include "Square_Matrix.h"

#include <vector>

using namespace std;

/**
 * @brief Allocates a matrix of size 0.
 */
Square_Matrix::Square_Matrix(void) { size = 0; matrix = NULL; }

/**
 * @brief Properly de allocates all of memory set asside for the matrix.
 */
Square_Matrix::~Square_Matrix(void) {
	// If destructor was called before Set_Size something bad might
	// happen, so I threw this in here just in case.
	if (size != 0) {
		// Delete the array held by each row pointer and then point it to null.
		for (int row = 0; row < size; ++row) {
			delete[] matrix[row];
			matrix[row] = NULL;
		}

		// and lastly delete matrix itself.
		delete[] matrix;

		// Set matrix to point to nullptr.
		matrix = NULL;

		// And reset size back to zero.
		size = 0;
	}
}

/**
 * @brief Constructs *this matrix using another matrix as reference.
 */
Square_Matrix::Square_Matrix(const Square_Matrix &other_matrix) {
	// Self assignment test
	if (this == &other_matrix)
		return;

	// This huge section just copied from destructor manually to make sure
	// this->~Square_Matrix() is not doing anything funky.
	if (other_matrix.size < 0)
		return;

	// Make the rows
	this->matrix = new int*[other_matrix.size];

	// Change the size of this array to the new correct size.
	this->size = other_matrix.size;

	// Fill the rows with new data.
	for (int i = 0; i < other_matrix.size; ++i) {
		this->matrix[i] = new int[other_matrix.size];

		for (int j = 0; j < other_matrix.size; ++j)
			this->matrix[i][j] = other_matrix.matrix[i][j];
	}
}

/**
 * @brief Sets the size of the square matrix, resize will reset contents.
 * 
 * @details Description taken from assignment requirments.
 * Allocates memory for the square matrix of given size (new_size) and 
 * sets the size of matrix to it. If the object already stored matrix 
 * all the values in it are lost.
 * 
 * Avoid memory leaks! If memory for the matrix was allocated before, 
 * make sure your properly release it before allocating more memory for
 * new matrix size! 
 * 
 * @param new_size [size of an edge.]
 */
void Square_Matrix::Set_Size(const int &new_size) {
	// We can't have a negative sized matrix. If new_size wasn't
	// required to be a const I would flip it to a positive, but
	// I guess just doing nothing is sufficiant.
	if (new_size < 0)
		return;

	// "If the object already stored matrix all the values in it are lost."
	// Checks to see if Set_Size was already ran and if so, wipe matrix
	// contents and clear the pointers.
	if (new_size != size && size != 0) {
		// Delete the array held by each row pointer
		for (int row = 0; row < size; ++row) {
			delete[] matrix[row];
			matrix[row] = NULL;
		}

		// Set matrix to point to nullptr.
		delete[] matrix;

		// And reset size back to zero.
		matrix = NULL;
	}

	// Makes N pointers representing a column N rows high.
	/*
	*              |- *Column 0     ==> nothing
	*  **matrix ==>|            ...
	*              |- *Column n - 1 ==> nothing
	*/
	matrix = new int*[new_size];

	// Goes through each column pointer and assigns a pointer of n ints with
	// each set to 0 now.
	for (int i = 0; i < new_size; ++i) {
		// Allocate n ints to each column pointer, effectively making the row.
		/*
		*              |- *Column 0     ==> item 0 ... item n-1
		*  **matrix ==>|            ...
		*              |- *Column n - 1 ==> item 0 ... item n-1
		*/
		matrix[i] = new int[new_size];

		// Fills cells of each row with 0.
		for (int j = 0; j < new_size; ++j)
			matrix[i][j] = 0;
	}

	// Since we completed the generation of the column, we may now change the
	// size.
	size = new_size;
}

/**
 * @brief Gets the size of the matrix.
 * 
 * @return The size of an edge of the matrix.
 */
int Square_Matrix::Get_Size(void) { return size; }

/**
 * @brief Sets an element of the matrix to the given value.
 * 
 * @param new_value Value we change the element to.
 * @param x X position of the cell.
 * @param y Y position of the cell.
 */
void Square_Matrix::Set_Elem(const int &new_value, const int &x, 
							 const int &y) {
	this->matrix[x][y] = new_value;
}

/**
 * @brief Returns the contents of a cell at the specified row and column.
 * 
 * @param x X position of the cell.
 * @param y Y position of the cell.
 * 
 * @return Contents of the cell.
 */
int Square_Matrix::Get_Elem(const int &x, const int &y) {
	return  matrix[x][y];
}

/**
 * @brief Returns the contents of a column.
 * 
 * @details If out of bounds, return a correctly sized vector of 0's.
 * 
 * @param num_column Column we want.
 * 
 * @return Contents of column as a vector of int's.
 */
std::vector<int> Square_Matrix::Get_Column(const int &num_column) {
	vector<int> column;

	// If the column is out of range, return a vector containing
	// a size amount of zero's.
	if (num_column >= this->size)
		for (int i = 0; i < this->size; ++i)
			column.push_back(0);
	else
		for (int i = 0; i < this->size; ++i)
			column.push_back(this->matrix[num_column][i]);

	return column;
}

/**
 * @brief Returns the contents of a row.
 * 
 * @details If out of bounds, return a correctly sized vector of 0's.
 * 
 * @param num_row row we want.
 * 
 * @return Contents of row as a vector of int's.
 */
std::vector<int> Square_Matrix::Get_Row(const int &num_row) {
	vector<int> row;

	// If the row is out of range, return a vector containing
	// a size amount of zero's.
	if (num_row >= this->size)
		for (int i = 0; i < this->size; ++i)
			row.push_back(0);
	else
		for (int i = 0; i < this->size; ++i)
			row.push_back(this->matrix[i][num_row]);

	return row;
}

/**
 * @brief Fills an entire matrix with a value.
 * 
 * @param value The value to fill the matrix with.
 */
void Square_Matrix::fill(const int &value) {
	// Goes through the entire matrix and one by one sets each element
	// to the val.
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			matrix[i][j] = value;
}

/**
 * @brief Comparison of size and contents of the other matrix.
 * 
 * @param other_matrix The matrix we are comparing to.
 * 
 * @return True if the same, false if otherwise.
 */
bool Square_Matrix::operator==(const Square_Matrix &other_matrix) {
	if (this == &other_matrix)
		return 1;

	int other_matrix_size = other_matrix.size;
	if (size != other_matrix_size)
		return 0;

	// If we got to here, then both are same size so no need to check again.
	if (size == 0)
		return 1;

	// Goes through entire matrices and compares.
	for (int i = 0; i < size; ++i)
	for (int j = 0; j < size; ++j)
	if (matrix[i][j] != other_matrix.matrix[i][j])
		return 0;

	// We got this far, so that means they are the same size and/or equal to
	// zero, and have the same contents.
	return 1;
}

/**
 * @brief Adds the contents of a same sized matrix to *this. 
 * 
 * @details You are guaranteed that "+" operator will always receive correct input 
 * (two non-empty matrices of the same size).
 * 
 * @param other_matrix Matrix we are adding to this one.
 */
Square_Matrix &Square_Matrix::operator+(const Square_Matrix &other_matrix) {
	Square_Matrix *temp_matrix;
	temp_matrix = new Square_Matrix;

	temp_matrix->Set_Size(other_matrix.size);

	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j) {
			int val = this->matrix[i][j] + other_matrix.matrix[i][j];
			temp_matrix->matrix[i][j] = val;
		}

	return *temp_matrix;
}

/**
 * @brief Overwrites *this matrix with the other matrix.
 * 
 * @details A class usually sets this up automatically, but it is a good idea to
 * impliment it yourself to make sure all is well since when default it might break.
 * 
 * @param other_matrix Matrix we are getting data from.
 */
Square_Matrix &Square_Matrix::operator=(const Square_Matrix &other_matrix) {
	// Self assignment test
	if (this == &other_matrix)
		return *this;

	// This huge section just copied from destructor manually to make sure
	// this->~Square_Matrix() is not doing anything funky.
	if (other_matrix.size < 0)
		return *this;

	// Deallocates all of the previous square matrix.
	if (this->size != other_matrix.size && other_matrix.size != 0 && this->size != 0) {
		// Delete the array held by each row pointer
		for (int row = 0; row < this->size; ++row) {
			delete[] this->matrix[row];
			this->matrix[row] = NULL;
		}

		// and lastly delete matrix itself.
		delete[] this->matrix;

		this->matrix = NULL;
	}

	// Make the rows
	this->matrix = new int*[other_matrix.size];

	// Change the size of this array to the new correct size.
	this->size = other_matrix.size;

	// Fill the rows with new data.
	for (int i = 0; i < other_matrix.size; ++i) {
		this->matrix[i] = new int[other_matrix.size];

		for (int j = 0; j < other_matrix.size; ++j)
			this->matrix[i][j] = other_matrix.matrix[i][j];
	}

	return *this;
}
