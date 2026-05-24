#include "search.h"


void getWord(const std::vector<std::vector<char>> &mtx, std::vector<std::string> &arr, int x1, int y1, int x2, int y2) {
    size_t mx = (x2 > x1 ? x2 - x1 : y2 - y1) + 1;
    int dx = (x2 > x1 ? 1 : 0), dy = (y2 > y1 ? 1 : 0);
    std::string newStr = {};
    newStr.reserve(mx);
    char sym = 0;

    for (size_t i = 0; i < mx; i++) {
        sym = mtx[x1 + dx*i][y1 + dy*i];
        if (sym == '\0') break;
        newStr.push_back(sym);
    }

    if (sym != '\0') {
        if (wordSearch::checkDictionary(newStr)) arr.push_back(newStr);
        std::reverse(newStr.begin(), newStr.end());
        if (wordSearch::checkDictionary(newStr)) arr.push_back(newStr);
    }
}

void sectionSelect(const std::vector<std::vector<char>> &mtx, std::vector<std::string> &arr, int x1, int y1) {
    for (size_t d = 1; x1 + d < mtx[0].size(); d++) {
        getWord(mtx, arr, x1, y1, x1 + d, y1);
        if (y1 + d < mtx.size()) {
            getWord(mtx, arr, x1, y1, x1 + d, y1 + d);
        }
    }
    for (size_t d = 1; y1 + d < mtx.size(); d++) {
        getWord(mtx, arr, x1, y1, x1, y1 + d);
    }
}

std::vector<std::string> pointSelect(const std::vector<std::vector<char>> &mtx) {
    std::vector<std::string> arr = {};
    for (size_t y1 = 0; y1 < mtx.size(); y1++) {
        for (size_t x1 = 0; x1 < mtx[0].size(); x1++) {
            sectionSelect(mtx, arr, x1, y1);
        }
    }
    return arr;
}


namespace wordSearch {

bool checkDictionary(const std::string str) {
    static std::ifstream fin;
    static std::unordered_set <std::string> uset;
    static std::string buf;

    if (!fin.is_open()) {
        fin.open(PATH_TO_DIC, std::ios::in);
        if (!fin.is_open()) throw std::invalid_argument("The file could not be opened");
    }

    if (uset.empty()) {
        fin >> buf;
        while(!fin.eof()) {
            if(fin.bad()) throw std::runtime_error(std::string("Failed to read: ") + strerror(errno));
            
            else if(fin.fail()) {
                fin.clear();
                fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            uset.insert(buf);
            fin >> buf;
        }
    }
    
    else if (uset.find(str) != uset.end()) return true;

    return false;
}

std::vector<std::string> search(const std::vector<std::vector<char>> &mtx) {
    std::vector<std::string> arr = pointSelect(mtx);

    return arr;
}

}
