#include "UI.h"

using UI::manager;

int main() {
    int result = 0;
    try {
        result = manager();
    }
    catch(const std::range_error &error) {
        return 1;
    }
    catch(const std::exception &error) {
        std::cout << error.what() << std::endl;
        return 2;
    }

    return result;
}
