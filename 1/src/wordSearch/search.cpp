#include "search.h"

using differentMatrix::Matrix, differentMatrix::Strs;
using differentMatrix::createStrings, differentMatrix::addStrings, differentMatrix::getElement, differentMatrix::freeStrings;


void getWord(const Matrix &mtx, Strs &arr, size_t x1, size_t y1, size_t x2, size_t y2) {
    size_t mx = (x2 > x1 ? x2 - x1 : y2 - y1) + 1;
    int dx = (x2 > x1 ? 1 : 0), dy = (y2 > y1 ? 1 : 0);
    char *newStr = new char[mx + 1] {};
    char sym = 0;

    for (size_t i = 0; i < mx; i++) {
        sym = getElement(mtx, y1 + dy*i, x1 + dx*i);
        if (sym == '\0') break;
        newStr[i] = sym;
    }

    if (sym != '\0') {
        try {
            if (wordSearch::checkDictionary(newStr)) addStrings(arr, newStr);
            std::reverse(newStr, newStr + mx);
            if (wordSearch::checkDictionary(newStr)) addStrings(arr, newStr);
        }
        catch(...) {
            delete[] newStr;
            throw;
        }
    }
    delete[] newStr;
}

void sectionSelect(const Matrix &mtx, Strs &arr, size_t x1, size_t y1) {
    for (size_t d = 1; x1 + d < getCountColumn(mtx); d++) {
        getWord(mtx, arr, x1, y1, x1 + d, y1);
        if (y1 + d < getCountStrs(mtx)) {
            getWord(mtx, arr, x1, y1, x1 + d, y1 + d);
        }
    }
    for (size_t d = 1; y1 + d < getCountStrs(mtx); d++) {
        getWord(mtx, arr, x1, y1, x1, y1 + d);
    }
}

Strs *pointSelect(const Matrix &mtx) {
    Strs *arr = createStrings();
    for (size_t y1 = 0; y1 < getCountStrs(mtx); y1++) {
        for (size_t x1 = 0; x1 < getCountColumn(mtx); x1++) {
            sectionSelect(mtx, *arr, x1, y1);
        }
    }
    return arr;
}


namespace wordSearch {

Strs *search(const Matrix *mtx) {
    if (mtx == nullptr) throw std::logic_error("The value of the nullptr matrix");
    Strs *arr = pointSelect(*mtx);

    return arr;
}

}
