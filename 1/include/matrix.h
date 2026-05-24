#ifndef MATRIX_DEFENDER
#define MATRIX_DEFENDER

#include <iostream>
#include <algorithm>
#include <fstream>

/** 
 * @addtogroup libDifferentMatrix
 * @brief A library with self-made containers consisting of tables
 * 
 * At the moment, 2 containers have been implemented. 
 * 1 container is an array of strings with a self-allocated amount of memory. 
 * 2 The container is a rectangular matrix (a special case of an array of strings).
 * It requires linking the libUtilities library to work. 
 * @{
 */

/// @brief Interface for working with the Matrix structure and Strs structure
namespace differentMatrix {

struct Matrix;

/**
 * @brief Allocation of memory for the Matrix structure
 * 
 * Each line consists of n+1 characters, where n+1 character is equal to '\0'.
 * Therefore, you can work with each line as with a regular string: char*.
 * 
 * @param m The number of rows
 * @param n The number of columns
 * @return Matrix* the fields m, n correspond to the arguments, iterator = 0, all characters will have the state '\o'
 * @throw std::bad_alloc When memory is not allocated by the NEW operator
 */
Matrix *craeteMatrix(unsigned int m, unsigned int n);

/**
 * @brief Frees up the allocated memory with the function create
 * 
 * @param mtx Pointer to the matrix structure
 * @throw std::logic_error The value of the nullptr matrix
 */
void freeMatrix(Matrix *mtx);

/**
 * @brief Adds a character to the location that the iterator points to
 * 
 * Iterator is shifted by 1 character.
 * 
 * @param mtx Pointer to the matrix structure
 * @param x The symbol you need to insert
 */
void addMatrix(Matrix &mtx, char x);

/**
 * @brief Adds a character based on the x y coordinates
 * 
 * @param mtx Pointer to the matrix structure
 * @param c The symbol you need to insert
 * @param x X coordinate
 * @param y Y coordinate
 * @throw std::logic_error Invalid coordinate value in the matrix
 */
void addMatrix(Matrix &mtx, char c, unsigned int x, unsigned int y);

/**
 * @brief Get the number of rows, that is, the m field
 * 
 * @param mtx Pointer to the matrix structure
 * @return int Number of rows
 */
size_t getCountStrs(const Matrix &mtx);

/**
 * @brief Get the number of column, that is, the n field
 * 
 * @param mtx Pointer to the matrix structure
 * @return int Number of column
 */
size_t getCountColumn(const Matrix &mtx);

/**
 * @brief Get a symbol with x, y coordinates
 * 
 * @param mtx Pointer to the matrix structure
 * @param x X coordinate
 * @param y Y coordinate
 * @return char The symbol itself
 * @throw std::logic_error Invalid coordinate value in the matrix
 */
char getElement(const Matrix &mtx, unsigned int x, unsigned int y);

/**
 * @brief Matrix output in std::cout
 * 
 * @param mtx Pointer to the matrix structure
 * @throw std::logic_error The value of the nullptr matrix
 * 
 * @bug If '\0' is found in the row, then the output of this line will stop
 */
void printMatrix(const Matrix *mtx, std::ostream &fout);

}
/// @}

#endif
