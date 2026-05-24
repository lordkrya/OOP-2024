#include "UI.h"

namespace UI {

void printMatrix(const std::vector<std::vector<char>> &mtx) {
    std::for_each(mtx.begin(), mtx.end(), [] (const std::vector<char> &mtx) {for (char c: mtx) std::cout << c;
                                                                                std::cout << std::endl;});
}

void printStrings(const std::vector<std::string> &arr) {
    std::for_each(arr.begin(), arr.end(), [] (const std::string &arr) {std::cout << arr << std::endl;});
}

}
