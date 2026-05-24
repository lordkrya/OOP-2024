#ifndef UTILITIES_DEFENDER
#define UTILITIES_DEFENDER

#include <cstring>
#include <iostream>
#include <limits>

/**
 * @addtogroup libUtilities
 * @brief The library of utilities
 * 
 * The library implements the most basic functions needed in almost every project.
 * @{
 */

/// @brief A library with a set of useful functions used in the project
namespace utilities {

/**
 * @brief Reads a specific type of data from a stream
 * 
 * @tparam T The data type being read
 * @param fin Input stream
 * @param err The text that will be output to std::out when meeting: fin.fail().
 * @return T The read value
 * 
 * @throw std::runtime_error Failed to read
 * @throw std::range_error EOF received
 */
template<class T>
T getSth(std::istream &fin, std::string err = "") {
    T a;
    fin >> a;
    while(!fin.eof()) {
        if(fin.bad()) throw std::runtime_error(std::string("Failed to read: ") + strerror(errno));
        
        else if(fin.fail()) {
            fin.clear();
            fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!err.empty()) std::cout << err << std::endl;
        }

        else return a;
        fin >> a;
    }
    throw std::range_error("EOF received");
}

/**
 * @brief The self-written realloc() function
 * 
 * The original array will be deleted using delete[]. 
 * Make sure that you allocated memory using the NEW[] operator.
 * 
 * @tparam T The data type of the array
 * @param arr A pointer to the array to be expanded
 * @param oldSize The length of the array initially
 * @param newSize New array length
 * @return T* Pointer to the modified array
 * 
 * @throw std::logic_error A null pointer was received"
 * @throw std::logic_error Invalid memory allocation value
 * @throw std::logic_error Invalid value of allocated memory
 */
template <typename T>
T *resize(T *arr, size_t oldSize, size_t newSize) {
    if (arr == nullptr) throw std::logic_error("A null pointer was received");

    T *newArr = new T[newSize] {};
    std::copy(arr, arr + std::min(oldSize, newSize), newArr);

    delete[] arr;
    return newArr;
}

/**
 * @brief The self-written strdup() function
 * 
 * @param str A pointer to the original string
 * @return char* Copied string to a new memory area
 * 
 * @throw std::logic_error A null pointer was received
 */
char *newStrdup(const char *str);

}
/// @}

#endif
