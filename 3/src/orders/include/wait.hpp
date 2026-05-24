#ifndef WAIT_DEFENDER
#define WAIT_DEFENDER

#include <iostream>

#include "baseOrder.hpp"

namespace orders {

class Wait: public Order {
    public:
        void executeOrder() const {
            std::cout << "wait" <<std::endl;
        }

        inline unsigned int getID() const noexcept {
            return 0;
        }

        inline std::string getName() const noexcept {
            return "Wait";
        }
};

}

#endif