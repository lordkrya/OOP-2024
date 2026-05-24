#ifndef ATTACK_DEFENDER
#define ATTACK_DEFENDER

#include <iostream>

#include "baseInstruction.hpp"

namespace instructions {

class Attack: public Instruction {
    public:
        void request() const {
            std::cout << "moveTo" <<std::endl;
        }

        inline unsigned int getRegulation() const noexcept {
            return 1;
        }

        inline std::string getName() const noexcept {
            return "Attack";
        }
};

}

#endif
