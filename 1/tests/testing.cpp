#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <sstream>
#include "../include/utilities.h"
#include "../include/matrix.h"
#include "../include/strings.h"
#include "../include/search.h"

using namespace utilities;
using namespace differentMatrix;
using namespace wordSearch;


TEST_CASE("utilities") {
    SECTION("getSth") {
        std::stringstream out("a");
        char a = getSth<char>(out);
        REQUIRE(a == 'a');
        out.setstate(std::ios::failbit);
        REQUIRE_THROWS(getSth<char>(out));
        out.setstate(std::ios::eofbit);
        REQUIRE_THROWS(getSth<char>(out));
        out.setstate(std::ios::badbit);
        REQUIRE_THROWS(getSth<char>(out));
    }

    SECTION("resize") {
        int *arr = nullptr;
        REQUIRE_THROWS(resize(arr, 1, 2));
        arr = new int[2] {1, 2};
        arr = resize(arr, 2, 3);
        REQUIRE(arr[0] == 1);
        REQUIRE(arr[1] == 2);
        REQUIRE(arr[2] == 0);
        arr = resize(arr, 3, 3);
        REQUIRE(arr[0] == 1);
        REQUIRE(arr[1] == 2);
        REQUIRE(arr[2] == 0);
        arr = resize(arr, 3, 1);
        arr = resize(arr, 1, 2);
        REQUIRE(arr[0] == 1);
        REQUIRE(arr[1] == 0);
        delete[] arr;
    }

    SECTION("newStrdup") {
        char *str = nullptr;
        REQUIRE_THROWS(str = newStrdup(str));
        str = newStrdup("\0");
        REQUIRE(str[0] == '\0');
        delete[] str;
        str = newStrdup("abc\0");
        REQUIRE(str[0] == 'a');
        REQUIRE(str[1] == 'b');
        REQUIRE(str[2] == 'c');
        REQUIRE(str[3] == '\0');
        delete[] str;
    }
}


TEST_CASE("differetMatrix") {
    SECTION("differetMatrixCreate") {
        REQUIRE_THROWS(freeMatrix(nullptr));
    }

    SECTION("differetMatrixFunction") {
        Matrix *mtx = craeteMatrix(2, 2);
        REQUIRE(getCountStrs(*mtx) == 2);
        REQUIRE(getCountColumn(*mtx) == 2);
        REQUIRE_THROWS(addMatrix(*mtx, 'a', -1, 1));
        REQUIRE_THROWS(addMatrix(*mtx, 'a', 1, 3));
        REQUIRE_THROWS(getElement(*mtx, -3, 1));
        REQUIRE_THROWS(getElement(*mtx, 1, 2));
        addMatrix(*mtx, 'a');
        addMatrix(*mtx, 'b', 0, 0);
        addMatrix(*mtx, 'c', 0, 1);
        addMatrix(*mtx, 'd');
        addMatrix(*mtx, 'e');
        REQUIRE(getElement(*mtx, 0, 0) == 'b');
        REQUIRE(getElement(*mtx, 1, 0) == 'd');
        REQUIRE(getElement(*mtx, 0, 1) == 'e');
        REQUIRE(getElement(*mtx, 1, 1) == '\0');
        addMatrix(*mtx, 'f');
        std::stringstream out;
        printMatrix(mtx, out);
        REQUIRE(out.str() == "bd\nef\n");
        addMatrix(*mtx, 'a');
        REQUIRE(getElement(*mtx, 0, 0) == 'a');
        freeMatrix(mtx);
    }

    SECTION("differetStrsCreate") {
        REQUIRE_THROWS(freeStrings(nullptr));
    }

    SECTION("differetStrsFunction") {
        Strs *arr = createStrings();
        REQUIRE(getNumStrings(*arr) == 0);
        REQUIRE_THROWS(getStrings(*arr, 0) == 0);
        addStrings(*arr, "one");
        std::stringstream out;
        printStrings(arr, out);
        REQUIRE(out.str() == "one\n");
        addStrings(*arr, "two");
        addStrings(*arr, "ten");
        REQUIRE(getNumStrings(*arr) == 3);
        REQUIRE_THROWS(getStrings(*arr, -1));
        REQUIRE(strcmp(getStrings(*arr, 0), "one") == 0);
        REQUIRE(strcmp(getStrings(*arr, 1), "two") == 0);
        REQUIRE(strcmp(getStrings(*arr, 2), "ten") == 0);
        REQUIRE_THROWS(getStrings(*arr, 3));
        addStrings(*arr, "4");
        addStrings(*arr, "5");
        addStrings(*arr, "6");
        addStrings(*arr, "7");
        addStrings(*arr, "8");
        addStrings(*arr, "9");
        REQUIRE(strcmp(getStrings(*arr, 8), "9") == 0);
    }
}


TEST_CASE("wordSearch") {
    SECTION("wordSearchSearch", "Custom") {
        REQUIRE_THROWS(search(nullptr));
        Matrix *mtx = craeteMatrix(4, 4);
        Strs *arr = search(mtx);
        REQUIRE(getNumStrings(*arr) == 0);
        freeStrings(arr);
        // Тесты для словаря лишь со словами one, two, ten (В примере '-' = '\0'):
        // o n e -
        // - t - -
        // - e w -
        // - n - o
        addMatrix(*mtx, 'o', 0, 0);
        addMatrix(*mtx, 'n', 1, 0);
        addMatrix(*mtx, 'e', 2, 0);
        addMatrix(*mtx, 't', 1, 1);
        addMatrix(*mtx, 'e', 1, 2);
        addMatrix(*mtx, 'n', 1, 3);
        addMatrix(*mtx, 'w', 2, 2);
        addMatrix(*mtx, 'o', 3, 3);
        arr = search(mtx);
        REQUIRE(getNumStrings(*arr) == 3);
        REQUIRE(strcmp(getStrings(*arr, 0), "one") == 0);
        REQUIRE(strcmp(getStrings(*arr, 1), "ten") == 0);
        REQUIRE(strcmp(getStrings(*arr, 2), "two") == 0);
        freeMatrix(mtx);
        freeStrings(arr);
    }

    SECTION("wordSearchSearch", "Std") {
        std::vector<std::vector<char>> mtx {};
        std::vector<std::string> arr = search(mtx);
        REQUIRE(arr.size() == 0);
        mtx = std::vector(4, std::vector<char>(4));
        arr = search(mtx);
        REQUIRE(arr.size() == 0);
        // Условия что и для SECTION("wordSearchSearch", "Custom")
        mtx[0][0] = 'o';
        mtx[0][1] = 'n';
        mtx[0][2] = 'e';
        mtx[1][1] = 't';
        mtx[2][1] = 'e';
        mtx[3][1] = 'n';
        mtx[2][2] = 'w';
        mtx[3][3] = 'o';
        arr = search(mtx);
        REQUIRE(arr.size() == 3);
        REQUIRE(arr[0] == "one");
        REQUIRE(arr[1] == "ten");
        REQUIRE(arr[2] == "two");
    }

    SECTION("wordSearchcheckDictionary") {
        REQUIRE(checkDictionary("\0") == false);
        REQUIRE(checkDictionary("\n") == false);
        REQUIRE(checkDictionary("\t") == false);
        REQUIRE(checkDictionary(" ") == false);
        REQUIRE(checkDictionary("one") == true);
        REQUIRE(checkDictionary("two") == true);
        REQUIRE(checkDictionary("ten") == true);
        REQUIRE(checkDictionary(" one") == false);
    }
}
