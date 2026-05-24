#ifndef EXPECTATION_DEFENDER
#define EXPECTATION_DEFENDER

#include <iostream>

#include "baseInstruction.hpp"

namespace instructions {

class Expectation: public Instruction {
    public:
        void request() const {
            std::cout << "wait" <<std::endl;
        }

        inline unsigned int getRegulation() const noexcept {
            return 0;
        }

        inline std::string getName() const noexcept {
            return "Expectation";
        }
};

}

#endif