#include "matrix.h"


namespace differentMatrix {

/**
 * @brief A matrix of m by n 
 * 
 * To work with the structure, use namespace: differentMatrix.
 * The matrix required for containing characters
 */
struct Matrix {
    char **data;
    size_t m;              ///< The number of rows
    size_t n;              ///< The number of columns
    unsigned int iterator = 0;   ///< The iterator is defined by the value: y_position = iterator/m, x_position = iterator mod m
};


Matrix *craeteMatrix(unsigned int m, unsigned int n) {
    Matrix *mtx = new Matrix {};
    
    try { 
        mtx->data = new char*[m] {};

        for (unsigned int i = 0; i < m; i++) {
            try {
                mtx->data[i] = new char[n + 1] {};
            }
            catch(...) {
                for (unsigned int j = 0; j < i; j++) delete[] mtx->data[j];
                throw;
            }
        }

        mtx->m = m;
        mtx->n = n;

        return mtx;
    }
    catch(...) {
        delete mtx;
        throw;
    }
}


void freeMatrix(Matrix *mtx) {
    if (mtx == nullptr) throw std::logic_error("The value of the nullptr matrix");
    std::for_each(mtx->data, mtx->data + mtx->m, [] (const char *str) {delete[] str;});

    delete[] mtx->data;
    delete mtx;
}


void addMatrix(Matrix &mtx, char x) {
    size_t i = mtx.iterator;
    mtx.data[i/mtx.n][i%mtx.n] = x;

    if (i == (mtx.m)*(mtx.n) - 1) {
        mtx.iterator = 0;
    } else {
        mtx.iterator++;
    }
}

void addMatrix(Matrix &mtx, char c, unsigned int x, unsigned int y) {
    if (y >= mtx.m || x >= mtx.n) throw std::logic_error("Invalid coordinate value in the matrix");
    mtx.data[y][x] = c;
}

size_t getCountStrs(const Matrix &mtx) {
    return mtx.m;
}

size_t getCountColumn(const Matrix &mtx) {
    return mtx.n;
}

char getElement(const Matrix &mtx, unsigned int x, unsigned int y) {
    if (y >= mtx.m || x >= mtx.n) throw std::logic_error("Invalid coordinate value in the matrix");
    return mtx.data[y][x];
}


void printMatrix(const Matrix *mtx, std::ostream &fout = std::cout) {
    if (mtx == nullptr) throw std::logic_error("The value of the nullptr matrix");
    std::for_each(mtx->data, mtx->data + mtx->m, [&fout] (const char* str) {fout << str << std::endl;});
}

}
