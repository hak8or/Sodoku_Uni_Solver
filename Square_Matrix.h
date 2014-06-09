/* 	
	Author: Marcin Ziemianowicz
	Class: CSCI 235 (Descrete Analysis 2) Section 01
	Professor: Pavel Shostak
*/
	
#ifndef Square_Matrix_H
#define Square_Matrix_H

#include <vector>

/**
 * @brief A square matrix of int's
 * 
 * @details Object of this class stores integer matrix of size NxN. This matrix
 * is represented as dynamically allocated two-dimentional array of integers
 */
class Square_Matrix {
public:
	/**
	 * @brief Allocates a matrix of size 0.
	 */
	Square_Matrix(void);

	/**
	 * @brief Properly de allocates all of memory set asside for the matrix.
	 */
	~Square_Matrix(void);

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
	void Set_Size (const int &new_size);

	/**
	 * @brief Gets the size of the matrix.
	 * 
	 * @return The size of an edge of the matrix.
	 */
	int Get_Size(void) const;

	/**
	 * @brief Sets an element of the matrix to the given value.
	 * 
	 * @param new_value Value we change the element to.
	 * @param x X position of the cell.
	 * @param y Y position of the cell.
	 */
	void Set_Elem (const int &new_value, const int &x, const int &y);

	/**
	 * @brief Returns the contents of a cell at the specified row and column.
	 * 
	 * @param x X position of the cell.
	 * @param y Y position of the cell.
	 * 
	 * @return Contents of the cell.
	 */
	int Get_Elem (const int &x, const int &y) const;

	/**
	 * @brief Returns the contents of a row.
	 * 
	 * @details If out of bounds, return a correctly sized vector of 0's.
	 * 
	 * @param num_row row we want.
	 * 
	 * @return Contents of row as a vector of int's.
	 */
	std::vector<int> Get_Row(const int &num_row) const;

	/**
	 * @brief Returns the contents of a column.
	 * 
	 * @details If out of bounds, return a correctly sized vector of 0's.
	 * 
	 * @param num_column Column we want.
	 * 
	 * @return Contents of column as a vector of int's.
	 */
	std::vector<int> Get_Column(const int &num_column) const;

	/**
	 * @brief Fills an entire matrix with a value.
	 * 
	 * @param value The value to fill the matrix with.
	 */
	void fill(const int &value);

	/**
	 * @brief Sanity test (For unit testing mostly).
	 * 
	 * @return 2014 as an int
	 */
	int inline sanity_check(void) const {return 2014;}

	/**
	 * @brief Comparison of size and contents of the other matrix.
	 * 
	 * @param other_matrix The matrix we are comparing to.
	 * 
	 * @return True if the same, false if otherwise.
	 */
	bool operator==(const Square_Matrix &other_matrix) const;

	/**
	 * @brief Adds the contents of a same sized matrix to *this. 
	 * 
	 * @details You are guaranteed that "+" operator will always receive correct input 
	 * (two non-empty matrices of the same size).
	 * 
	 * @param other_matrix Matrix we are adding to this one.
	 */
	Square_Matrix &operator+(const Square_Matrix &other_matrix);

	/**
	 * @brief Overwrites *this matrix with the other matrix.
	 * 
	 * @details A class usually sets this up automatically, but it is a good idea to
	 * impliment it yourself to make sure all is well since when default it might break.
	 * 
	 * @param other_matrix Matrix we are getting data from.
	 */
	Square_Matrix &operator=(const Square_Matrix &other_matrix);

	/**
	 * @brief Constructs *this matrix using another matrix as reference.
	 */
	Square_Matrix(const Square_Matrix &other_matrix);

private:
	// Size of the matrix, assuming matrix is a square.
	int size;

	// Pointer to pointer for matrix contents (rows, columns).
	int **matrix;
};
#endif
