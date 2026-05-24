#include "utilities.h"

namespace utilities {

char *newStrdup(const char *str) {
    if (str == nullptr) throw std::logic_error("A null pointer was received");

    int len = strlen(str);
    char *newStr = new char[len + 1] {};
    
    std::copy(str, str + len + 1, newStr);
    return newStr;
}

}
