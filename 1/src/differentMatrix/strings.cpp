#include "strings.h"

using utilities::newStrdup, utilities::resize;


namespace differentMatrix{

/**
 * @brief Array of strings
 * 
 * To work with the structure, use namespace: differentMatrix.
 * You will not need to pay attention to memory reallocation.
 */
struct Strs {
    char **data;
    size_t size = 0;                       //< Number of strings
    size_t capacity = DEFAULT_CAPACITY;    //< Minimum allocated memory for an array of strings
};


Strs *createStrings() {
    Strs *arr = new Strs {};
    try {
        arr->data = new char*[DEFAULT_CAPACITY] {};
    }
    catch (...) {
        delete arr;
        throw;
    }

    return arr;
}

void freeStrings(Strs *arr) {
    if (arr == nullptr) throw std::logic_error("The value of the nullptr strings");
    std::for_each(arr->data, arr->data + arr->size, [] (const char *str) {delete[] str;});

    delete[] arr->data;
    delete arr;
}


void addStrings(Strs &arr, const char *str) {
    if (str == nullptr) throw std::logic_error("The value of the nullptr string");

    if (arr.size == arr.capacity) {
        arr.data = resize<char*>(arr.data, arr.capacity, arr.capacity*2);
        arr.capacity *= 2;
    }

    arr.data[arr.size] = newStrdup(str);
    arr.size++;
}

char *getStrings(const Strs &arr, unsigned int num) {
    if (num >= arr.size) throw std::logic_error("Invalid string number");
    return arr.data[num];
}

size_t getNumStrings(const Strs &arr) {
    return arr.size;
}

void printStrings(const Strs *arr, std::ostream &fout = std::cout) {
    if (arr == nullptr) throw std::logic_error("The value of the nullptr strings");
    std::for_each(arr->data, arr->data + arr->size, [&fout] (const char *str) {fout << str << std::endl;});
}

}
