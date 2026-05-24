#include "UI.h"

#ifndef SEARCHMOD_UI
using differentMatrix::Matrix, differentMatrix::Strs;
using differentMatrix::craeteMatrix, differentMatrix::printMatrix, differentMatrix::addMatrix, differentMatrix::printStrings;
#endif

using wordSearch::search;
using utilities::getSth;


auto genMatrix(std::istream &fin) {
    int m = 0, n = 0;
    char a = 0;
    if (&fin == &std::cin) std::cout << "Enter the number of rows and columns of the matrix:" << std::endl;
    m = getSth<int>(fin, "Error entering the count of lines");
    n = getSth<int>(fin, "Error entering the count of columns");

    #ifndef SEARCHMOD_UI
    Matrix *mtx = craeteMatrix(m, n);
    #else
    std::vector<std::vector<char>> mtx(m, std::vector<char>(n));
    #endif

    try {
        if (&fin == &std::cin) std::cout << "Enter the matrix:" << std::endl;
        for (int i = 0; i < m*n; i++) {
            a = getSth<char>(fin, "Character input error");

            #ifndef SEARCHMOD_UI
            addMatrix(*mtx, a, i%n, i/n); // Или использовать итератор: addMatrix(*mtx, a);
            #else
            mtx[i/n][i%n] = a;
            #endif
        }

    }
    catch(...) {
        #ifndef SEARCHMOD_UI
        freeMatrix(mtx);
        #endif
        throw;
    }
    return mtx;
}


namespace UI {

int manager() {
    #ifndef SEARCHMOD_UI
    std::cout << "The program is compiled without a flag: SEARCHMOD_UI\n" << std::endl;
    Matrix *mtx = nullptr;
    Strs *arr = nullptr;
    #else
    std::vector<std::vector<char>> mtx = {};
    std::vector<std::string> arr = {};
    #endif

    int menu = 0;
    std::ifstream fin;
    std::string nameFile;

    std::cout << "Select the operation ID:\n\
[1] Manual input\n\
[2] Reading from a file\n\
To terminate, use EOF" << std::endl;

    if ((menu = getSth<int>(std::cin, "Integer reading error"))) {
        switch (menu) {
            case 1:
                mtx = genMatrix(std::cin);
                break;

            case 2:
                std::cout << "Enter the file name:" << std::endl;
                nameFile = getSth<std::string>(std::cin, "String reading error");
                fin.open(nameFile, std::ios::in);

                while (!fin.is_open()) {
                    std::cout << "File opening error" << std::endl;
                    nameFile = getSth<std::string>(std::cin, "String reading error");
                    fin.open(nameFile, std::ios::in);
                }

                mtx = genMatrix(fin);
                fin.close();
                break;
            
            default:
                std::cout << "Unknown operation id" << std::endl;
                return 1;
        }


        std::cout << "\n---The resulting matrix---\n";
        printMatrix(mtx);
        std::cout << std::endl;

        try {
            arr = search(mtx);
        }
        catch(...) {
            #ifndef SEARCHMOD_UI
            freeMatrix(mtx);
            #endif
            throw;
        }

        std::cout << "---Received words---\n";
        printStrings(arr);
        std::cout << std::endl;

        #ifndef SEARCHMOD_UI
        freeMatrix(mtx);
        freeStrings(arr);
        #endif
    }
    return 0;
}

}
