#ifndef STRINGS_DEFENDER
#define STRINGS_DEFENDER

#define DEFAULT_CAPACITY 8

#include <iostream>
#include <algorithm>
#include <fstream>

#include "utilities.h"

/** 
 * @addtogroup libDifferentMatrix
 * @{
 */

namespace differentMatrix {

struct Strs;

/**
 * @brief Allocation of memory for the Strs structure
 * 
 * Pointers to the strings themselves are equal to nullptr.
 * DEFAULT_CAPACITY cells are allocated for an array of strings. 
 * Initially, the value of DEFAULT_CAPACITY is 8.
 * 
 * @return Strs* Pointer to strings structure, field size = 0
 */
Strs *createStrings();

/**
 * @brief Frees up the allocated memory with the function create
 * 
 * @param arr Pointer to the strings structure
 * @throw std::logic_error The value of the nullptr strings
 */
void freeStrings(Strs *arr);


/**
 * @brief Adds a string to the end of the array
 * 
 * When inserting, the size field increases by 1.
 * The capacity field increases by 2 times in case of an overflow of the array.
 * 
 * @param arr Pointer to the strings structure
 * @param str The string that needs to be added
 * @throw std::logic_error The value of the nullptr string
 */
void addStrings(Strs &arr, const char *str);


/**
 * @brief Get a string by index
 * 
 * @param arr Pointer to the strings structure
 * @param num string index
 * @return char* The resulting string
 * @throw std::logic_error Invalid string number
 */
char *getStrings(const Strs &arr, unsigned int num);

/**
 * @brief Get the number of strings
 * 
 * @param arr Pointer to the strings structure
 * @return int Number of strings
 */
size_t getNumStrings(const Strs &arr);

/**
 * @brief Strings output in std::cout
 * 
 * @param arr Pointer to the strings structure
 * @throw std::logic_error The value of the nullptr strings
 */
void printStrings(const Strs *arr, std::ostream &fout);

}
/// @} End of Doxygen Groups

#endif
