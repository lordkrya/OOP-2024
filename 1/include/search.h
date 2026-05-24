#ifndef SEARCH_DEFENDER
#define SEARCH_DEFENDER

#define PATH_TO_DIC "../share/dictionary.txt"
//#define PATH_TO_DIC "/media/sf_shared-folder/oop2024/1/share/dictionary.txt"

#include <unordered_set>
#include <fstream>
#include <vector>

#include "matrix.h"
#include "strings.h"


/**
 * @addtogroup libWordSearch
 * @brief Word search library
 * 
 * To connect the library correctly, 
 * make sure that you have specified the correct path to the dictionary 
 * in the PATH_TO_DIC variable in the src/wordSearch/search.h file.
 * It requires linking the libDifferentMatrix library to work. 
 * @{
 */

/// @brief Namespace for searching for words in the matrix and checking the existence of words in the dictionary
namespace wordSearch {

/**
 * @brief Word search function
 * 
 * To work with a matrix and an array of strings, use the differentMatrix namespace.
 * 
 * @param mtx The matrix of symbols
 * @return differentMatrix::Strs* Array of strings
 * 
 * @throw std::logic_error The value of the nullptr matrix
 * @throw std::invalid_argument The file could not be opened
 * @throw std::runtime_error Failed to read from file
 */
differentMatrix::Strs *search(const differentMatrix::Matrix *mtx);

/**
 * @brief Word search function
 * 
 * To work with a vector of vectors and the vector of strings, use the std namespace.
 * 
 * @param mtx The matrix of symbols
 * @return std::vector<std::string> Vector of strings
 * 
 * @throw std::invalid_argument The file could not be opened
 * @throw std::runtime_error Failed to read from file
 */
std::vector<std::string> search(const std::vector<std::vector<char>> &mtx);

/**
 * @brief Checks if a word exists in the dictionary
 * 
 * The path to the dictionary is specified using the PATH_TO_DIC.
 * Variable located in the file: src/wordSearch/search.h
 * 
 * @param str The word you are looking for
 * @return true The word exists
 * @return false The word was not found
 * 
 * @throw std::invalid_argument The file could not be opened
 * @throw std::runtime_error Failed to read from file
 */
bool checkDictionary(const std::string str);

}
/// @}

#endif
