#include "Sodoku.h"
#include "Square_Matrix.h"

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

// Throws out the current sodoku puzzle's contents.
void sodoku::display(void){

}
